/*
 * LINTSPEED v2.0 - Core Engine
 * Pure C Network Performance Testing
 * 
 * Copyright (c) 2025 0xb0rn3
 * MIT License
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <ncurses.h>

#define VERSION "2.0.0"
#define MAX_SAMPLES 50
#define MAX_PACKET_SIZE 65536
#define DEFAULT_SERVER "1.1.1.1"
#define TIMEOUT_SEC 2

// Color pairs for ncurses
#define COLOR_HEADER 1
#define COLOR_SUCCESS 2
#define COLOR_WARNING 3
#define COLOR_ERROR 4
#define COLOR_INFO 5

// Network test results
typedef struct {
    double download_speed;
    double upload_speed;
    double ping_avg;
    double ping_min;
    double ping_max;
    double jitter;
    double packet_loss;
    int samples_sent;
    int samples_received;
    char server[256];
    char interface[64];
    char interface_type[128];
    char hardware[256];
    int link_speed;
    time_t timestamp;
} test_results_t;

// Global state
typedef struct {
    test_results_t results;
    int testing;
    int progress;
    char status[256];
    pthread_mutex_t lock;
    int use_icmp;
    int quick_mode;
} app_state_t;

static app_state_t state;
static volatile sig_atomic_t running = 1;

// Signal handler
void handle_signal(int sig) {
    running = 0;
}

// Get microsecond timestamp
static inline uint64_t get_usec(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

// Calculate checksum for ICMP
uint16_t checksum(void *data, int len) {
    uint32_t sum = 0;
    uint16_t *ptr = data;
    
    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }
    
    if (len == 1) {
        sum += *(uint8_t *)ptr;
    }
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    
    return ~sum;
}

// Get interface information
void detect_interface(void) {
    FILE *fp;
    char line[256];
    char iface[64] = "";
    int max_speed = 0;
    
    // Find the active interface with highest speed
    fp = popen("ip -o link show | grep 'state UP' | awk '{print $2}' | sed 's/://g'", "r");
    if (!fp) return;
    
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "lo") == 0) continue;
        
        char speed_path[256];
        snprintf(speed_path, sizeof(speed_path), "/sys/class/net/%s/speed", line);
        
        FILE *sf = fopen(speed_path, "r");
        if (sf) {
            int speed;
            if (fscanf(sf, "%d", &speed) == 1 && speed > max_speed) {
                max_speed = speed;
                strncpy(iface, line, sizeof(iface) - 1);
            }
            fclose(sf);
        }
    }
    pclose(fp);
    
    if (iface[0]) {
        pthread_mutex_lock(&state.lock);
        strncpy(state.results.interface, iface, sizeof(state.results.interface) - 1);
        state.results.link_speed = max_speed;
        
        // Determine interface type
        if (strncmp(iface, "wl", 2) == 0) {
            snprintf(state.results.interface_type, sizeof(state.results.interface_type), 
                    "WiFi/WLAN");
        } else if (max_speed == 1000) {
            snprintf(state.results.interface_type, sizeof(state.results.interface_type),
                    "Ethernet - Gigabit (Cat 5e/6/6a)");
        } else if (max_speed == 10000) {
            snprintf(state.results.interface_type, sizeof(state.results.interface_type),
                    "Ethernet - 10 Gigabit (Cat 6a/7)");
        } else if (max_speed == 100) {
            snprintf(state.results.interface_type, sizeof(state.results.interface_type),
                    "Ethernet - Fast Ethernet (Cat 5/5e)");
        } else {
            snprintf(state.results.interface_type, sizeof(state.results.interface_type),
                    "Ethernet - %d Mbps", max_speed);
        }
        
        // Get hardware info
        char vendor_path[256], device_path[256];
        snprintf(vendor_path, sizeof(vendor_path), "/sys/class/net/%s/device/vendor", iface);
        snprintf(device_path, sizeof(device_path), "/sys/class/net/%s/device/device", iface);
        
        FILE *vf = fopen(vendor_path, "r");
        FILE *df = fopen(device_path, "r");
        
        if (vf && df) {
            char vendor[32], device[32];
            if (fgets(vendor, sizeof(vendor), vf) && fgets(device, sizeof(device), df)) {
                vendor[strcspn(vendor, "\n")] = 0;
                device[strcspn(device, "\n")] = 0;
                
                char cmd[512];
                snprintf(cmd, sizeof(cmd), 
                        "lspci -d %s:%s 2>/dev/null | cut -d' ' -f2-",
                        vendor + 2, device + 2);
                
                FILE *pci = popen(cmd, "r");
                if (pci) {
                    if (fgets(state.results.hardware, sizeof(state.results.hardware), pci)) {
                        state.results.hardware[strcspn(state.results.hardware, "\n")] = 0;
                    }
                    pclose(pci);
                }
            }
        }
        
        if (vf) fclose(vf);
        if (df) fclose(df);
        
        pthread_mutex_unlock(&state.lock);
    }
}

// ICMP ping measurement
int icmp_ping(const char *host, double *rtt_ms) {
    int sock;
    struct sockaddr_in addr;
    struct icmp icmp_hdr;
    char packet[64];
    uint64_t start, end;
    int ret;
    
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        return -1;
    }
    
    struct timeval tv = {TIMEOUT_SEC, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, host, &addr.sin_addr);
    
    memset(&icmp_hdr, 0, sizeof(icmp_hdr));
    icmp_hdr.icmp_type = ICMP_ECHO;
    icmp_hdr.icmp_code = 0;
    icmp_hdr.icmp_id = getpid();
    icmp_hdr.icmp_seq = 1;
    icmp_hdr.icmp_cksum = 0;
    icmp_hdr.icmp_cksum = checksum(&icmp_hdr, sizeof(icmp_hdr));
    
    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
    
    start = get_usec();
    ret = sendto(sock, packet, sizeof(icmp_hdr), 0,
                 (struct sockaddr *)&addr, sizeof(addr));
    
    if (ret < 0) {
        close(sock);
        return -1;
    }
    
    char recv_buf[MAX_PACKET_SIZE];
    ret = recvfrom(sock, recv_buf, sizeof(recv_buf), 0, NULL, NULL);
    end = get_usec();
    
    close(sock);
    
    if (ret < 0) {
        return -1;
    }
    
    *rtt_ms = (end - start) / 1000.0;
    return 0;
}

// TCP ping fallback
int tcp_ping(const char *host, int port, double *rtt_ms) {
    int sock;
    struct sockaddr_in addr;
    uint64_t start, end;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    
    struct timeval tv = {TIMEOUT_SEC, 0};
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);
    
    start = get_usec();
    int ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    end = get_usec();
    
    close(sock);
    
    if (ret < 0) {
        return -1;
    }
    
    *rtt_ms = (end - start) / 1000.0;
    return 0;
}

// Ping measurement
void measure_ping(const char *host) {
    double samples[MAX_SAMPLES];
    int count = state.quick_mode ? 10 : 20;
    int received = 0;
    
    pthread_mutex_lock(&state.lock);
    snprintf(state.status, sizeof(state.status), "Testing ping...");
    state.progress = 0;
    pthread_mutex_unlock(&state.lock);
    
    for (int i = 0; i < count; i++) {
        double rtt;
        int ret;
        
        if (state.use_icmp) {
            ret = icmp_ping(host, &rtt);
            if (ret < 0) {
                state.use_icmp = 0;
                ret = tcp_ping(host, 80, &rtt);
            }
        } else {
            ret = tcp_ping(host, 80, &rtt);
        }
        
        if (ret == 0) {
            samples[received++] = rtt;
        }
        
        pthread_mutex_lock(&state.lock);
        state.progress = (i + 1) * 25 / count;
        snprintf(state.status, sizeof(state.status), 
                "Testing ping... %d/%d", i + 1, count);
        pthread_mutex_unlock(&state.lock);
        
        usleep(50000);
    }
    
    if (received > 0) {
        double sum = 0, sum_sq = 0;
        double min = samples[0], max = samples[0];
        
        for (int i = 0; i < received; i++) {
            sum += samples[i];
            sum_sq += samples[i] * samples[i];
            if (samples[i] < min) min = samples[i];
            if (samples[i] > max) max = samples[i];
        }
        
        pthread_mutex_lock(&state.lock);
        state.results.ping_avg = sum / received;
        state.results.ping_min = min;
        state.results.ping_max = max;
        state.results.jitter = received > 1 ? 
            sqrt((sum_sq / received) - (state.results.ping_avg * state.results.ping_avg)) : 0;
        state.results.packet_loss = ((count - received) * 100.0) / count;
        state.results.samples_sent = count;
        state.results.samples_received = received;
        pthread_mutex_unlock(&state.lock);
    }
}

// HTTP GET request
ssize_t http_get(const char *host, const char *path, char *response, size_t max_len) {
    int sock;
    struct sockaddr_in addr;
    struct hostent *he;
    char request[1024];
    ssize_t total = 0;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;
    
    struct timeval tv = {15, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    
    he = gethostbyname(host);
    if (!he) {
        close(sock);
        return -1;
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);
    
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }
    
    snprintf(request, sizeof(request),
            "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: lintspeed/2.0\r\n"
            "Connection: close\r\n"
            "\r\n", path, host);
    
    if (send(sock, request, strlen(request), 0) < 0) {
        close(sock);
        return -1;
    }
    
    ssize_t n;
    while ((n = recv(sock, response + total, max_len - total - 1, 0)) > 0) {
        total += n;
        if (total >= max_len - 1) break;
    }
    
    close(sock);
    response[total] = '\0';
    
    return total;
}

// Download speed test
void measure_download(void) {
    const char *test_urls[] = {
        "speedtest.ftp.otenet.gr|/files/test1Mb.db",
        "speedtest.ftp.otenet.gr|/files/test10Mb.db",
        "speedtest.ftp.otenet.gr|/files/test10Mb.db"
    };
    int num_tests = state.quick_mode ? 2 : 3;
    double max_speed = 0;
    
    for (int i = 0; i < num_tests; i++) {
        pthread_mutex_lock(&state.lock);
        state.progress = 25 + (i * 37) / num_tests;
        snprintf(state.status, sizeof(state.status), 
                "Testing download... %d/%d", i + 1, num_tests);
        pthread_mutex_unlock(&state.lock);
        
        char host[256], path[256];
        char *pipe = strchr(test_urls[i], '|');
        if (!pipe) continue;
        
        size_t host_len = pipe - test_urls[i];
        memcpy(host, test_urls[i], host_len);
        host[host_len] = '\0';
        strcpy(path, pipe + 1);
        
        char *response = malloc(20 * 1024 * 1024);
        if (!response) continue;
        
        uint64_t start = get_usec();
        ssize_t size = http_get(host, path, response, 20 * 1024 * 1024);
        uint64_t end = get_usec();
        
        free(response);
        
        if (size > 0) {
            double duration = (end - start) / 1000000.0;
            double speed = (size * 8.0) / (duration * 1000000.0);
            if (speed > max_speed) max_speed = speed;
        }
    }
    
    pthread_mutex_lock(&state.lock);
    state.results.download_speed = max_speed;
    pthread_mutex_unlock(&state.lock);
}

// Upload speed test
void measure_upload(void) {
    const size_t sizes[] = {102400, 512000, 1048576};
    int num_tests = state.quick_mode ? 2 : 3;
    double max_speed = 0;
    
    for (int i = 0; i < num_tests; i++) {
        pthread_mutex_lock(&state.lock);
        state.progress = 62 + (i * 38) / num_tests;
        snprintf(state.status, sizeof(state.status), 
                "Testing upload... %d/%d", i + 1, num_tests);
        pthread_mutex_unlock(&state.lock);
        
        char *data = malloc(sizes[i]);
        if (!data) continue;
        memset(data, 'x', sizes[i]);
        
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            free(data);
            continue;
        }
        
        struct timeval tv = {15, 0};
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
        
        struct sockaddr_in addr;
        struct hostent *he = gethostbyname("httpbin.org");
        if (he) {
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(80);
            memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);
            
            if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
                char header[512];
                snprintf(header, sizeof(header),
                        "POST /post HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Content-Length: %zu\r\n"
                        "Connection: close\r\n"
                        "\r\n", sizes[i]);
                
                uint64_t start = get_usec();
                send(sock, header, strlen(header), 0);
                send(sock, data, sizes[i], 0);
                uint64_t end = get_usec();
                
                double duration = (end - start) / 1000000.0;
                double speed = (sizes[i] * 8.0) / (duration * 1000000.0);
                if (speed > max_speed) max_speed = speed;
            }
        }
        
        close(sock);
        free(data);
    }
    
    pthread_mutex_lock(&state.lock);
    state.results.upload_speed = max_speed;
    pthread_mutex_unlock(&state.lock);
}

// Main test thread
void *test_thread(void *arg) {
    const char *server = (const char *)arg;
    
    detect_interface();
    measure_ping(server);
    measure_download();
    measure_upload();
    
    pthread_mutex_lock(&state.lock);
    state.progress = 100;
    snprintf(state.status, sizeof(state.status), "Test complete");
    state.testing = 0;
    state.results.timestamp = time(NULL);
    pthread_mutex_unlock(&state.lock);
    
    return NULL;
}

// Initialize ncurses
void init_ui(void) {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    init_pair(COLOR_HEADER, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_WARNING, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_ERROR, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_INFO, COLOR_BLUE, COLOR_BLACK);
}

// Draw the UI
void draw_ui(void) {
    clear();
    
    int row = 0;
    int cols = COLS;
    
    // Header
    attron(COLOR_PAIR(COLOR_HEADER) | A_BOLD);
    for (int i = 0; i < cols; i++) mvaddch(row, i, '=');
    row++;
    mvprintw(row++, (cols - 25) / 2, "LINTSPEED v%s", VERSION);
    mvprintw(row++, (cols - 20) / 2, "by 0xbv1 | 0xb0rn3");
    for (int i = 0; i < cols; i++) mvaddch(row++, i, '=');
    attroff(COLOR_PAIR(COLOR_HEADER) | A_BOLD);
    row++;
    
    pthread_mutex_lock(&state.lock);
    
    // Hardware info
    if (state.results.interface[0]) {
        attron(COLOR_PAIR(COLOR_INFO));
        mvprintw(row++, 2, "Interface: %s (%s)", 
                state.results.interface, state.results.interface_type);
        if (state.results.hardware[0]) {
            mvprintw(row++, 2, "Hardware: %s", state.results.hardware);
        }
        if (state.results.link_speed > 0) {
            mvprintw(row++, 2, "Link Speed: %d Mbps", state.results.link_speed);
        }
        attroff(COLOR_PAIR(COLOR_INFO));
        row++;
    }
    
    // Statistics
    attron(A_BOLD);
    for (int i = 0; i < cols; i++) mvaddch(row, i, '-');
    row++;
    mvprintw(row++, (cols - 18) / 2, "NETWORK STATISTICS");
    for (int i = 0; i < cols; i++) mvaddch(row++, i, '-');
    attroff(A_BOLD);
    row++;
    
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(row++, 2, "Download Speed:    %.2f Mbps", state.results.download_speed);
    mvprintw(row++, 2, "Upload Speed:      %.2f Mbps", state.results.upload_speed);
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    attron(COLOR_PAIR(COLOR_INFO));
    mvprintw(row++, 2, "Ping:              %.2f ms (min: %.2f, max: %.2f)", 
            state.results.ping_avg, state.results.ping_min, state.results.ping_max);
    mvprintw(row++, 2, "Jitter:            %.2f ms", state.results.jitter);
    attroff(COLOR_PAIR(COLOR_INFO));
    
    if (state.results.packet_loss > 5.0) {
        attron(COLOR_PAIR(COLOR_WARNING) | A_BOLD);
        mvprintw(row++, 2, "Packet Loss:       %.1f%% [!]", state.results.packet_loss);
        attroff(COLOR_PAIR(COLOR_WARNING) | A_BOLD);
    } else {
        attron(COLOR_PAIR(COLOR_SUCCESS));
        mvprintw(row++, 2, "Packet Loss:       %.1f%% [OK]", state.results.packet_loss);
        attroff(COLOR_PAIR(COLOR_SUCCESS));
    }
    row++;
    
    // Progress bar
    if (state.testing) {
        for (int i = 0; i < cols; i++) mvaddch(row, i, '-');
        row++;
        mvprintw(row++, (cols - 13) / 2, "TEST PROGRESS");
        for (int i = 0; i < cols; i++) mvaddch(row++, i, '-');
        row++;
        
        int bar_width = cols - 10;
        int filled = (bar_width * state.progress) / 100;
        
        mvaddch(row, 2, '[');
        for (int i = 0; i < bar_width; i++) {
            if (i < filled) {
                attron(COLOR_PAIR(COLOR_SUCCESS));
                mvaddch(row, 3 + i, ACS_BLOCK);
                attroff(COLOR_PAIR(COLOR_SUCCESS));
            } else {
                mvaddch(row, 3 + i, ACS_CKBOARD);
            }
        }
        mvaddch(row, 3 + bar_width, ']');
        mvprintw(row, 5 + bar_width, " %d%%", state.progress);
        row += 2;
        
        mvprintw(row++, 2, "Status: %s", state.status);
        row++;
    }
    
    pthread_mutex_unlock(&state.lock);
    
    // Controls
    for (int i = 0; i < cols; i++) mvaddch(row, i, '-');
    row++;
    attron(A_BOLD);
    mvprintw(row++, (cols - 8) / 2, "CONTROLS");
    attroff(A_BOLD);
    for (int i = 0; i < cols; i++) mvaddch(row++, i, '-');
    row++;
    
    mvprintw(row++, (cols - 55) / 2, 
            "[ENTER] Run Test  |  [R] Reset  |  [Q] Quit");
    row++;
    
    // Footer
    for (int i = 0; i < cols; i++) mvaddch(row, i, '=');
    row++;
    
    char footer[256];
    snprintf(footer, sizeof(footer), "Server: %s", state.results.server);
    mvprintw(row, 2, "%s", footer);
    
    char timestamp[64];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    mvprintw(row, cols - strlen(timestamp) - 2, "%s", timestamp);
    
    refresh();
}

// Main function
int main(int argc, char **argv) {
    memset(&state, 0, sizeof(state));
    pthread_mutex_init(&state.lock, NULL);
    strcpy(state.results.server, DEFAULT_SERVER);
    snprintf(state.status, sizeof(state.status), "Ready");
    
    state.use_icmp = (getenv("LINTSPEED_NO_ICMP") == NULL);
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quick") == 0) {
            state.quick_mode = 1;
        } else if ((strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--server") == 0) && i + 1 < argc) {
            strncpy(state.results.server, argv[++i], sizeof(state.results.server) - 1);
        }
    }
    
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    init_ui();
    
    pthread_t thread;
    
    while (running) {
        draw_ui();
        
        int ch = getch();
        
        if (ch == 'q' || ch == 'Q') {
            break;
        } else if ((ch == '\n' || ch == '\r') && !state.testing) {
            state.testing = 1;
            state.progress = 0;
            pthread_create(&thread, NULL, test_thread, state.results.server);
            pthread_detach(thread);
        } else if ((ch == 'r' || ch == 'R') && !state.testing) {
            memset(&state.results, 0, sizeof(state.results));
            strcpy(state.results.server, DEFAULT_SERVER);
        }
        
        usleep(100000);
    }
    
    endwin();
    pthread_mutex_destroy(&state.lock);
    
    printf("\n\033[1;32m[✓]\033[0m Thank you for using lintspeed v%s!\n\n", VERSION);
    
    return 0;
}
