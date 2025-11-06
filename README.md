# 🚀 lintspeed

> A powerful, accurate network speed test tool with a beautiful TUI for Linux terminal

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Linux](https://img.shields.io/badge/Platform-Linux-blue.svg)](https://www.linux.org/)
[![Python](https://img.shields.io/badge/Python-3.6+-green.svg)](https://www.python.org/)

## ✨ Features

- **🎯 100% Accurate Measurements** - Hardware-direct speed testing
- **📊 Comprehensive Metrics** - Download, Upload, Ping, Jitter, Packet Loss
- **🎨 Beautiful TUI** - Color-coded interface with real-time updates
- **⚡ Zero Configuration** - Auto-detects distro and installs dependencies
- **🔧 One-Command Setup** - Self-contained bash wrapper with embedded Python
- **📦 System-Wide Installation** - Optional installation to `/usr/local/bin/`
- **🌐 Multi-Server Testing** - Uses multiple endpoints for accuracy

## 🖼️ Screenshot

```
════════════════════════════════════════════════════════════════════
                        LINTSPEED v1.0
                     by 0xbv1 | 0xb0rn3
════════════════════════════════════════════════════════════════════
┌─ NETWORK STATISTICS ─────────────────────────────────────────────┐
│                                                                    │
│  Download: 156.42 Mbps           Jitter: 2.34 ms                 │
│  Upload: 89.76 Mbps              Packet Loss: 0.0%               │
│  Ping: 12.45 ms                                                   │
│                                                                    │
└────────────────────────────────────────────────────────────────────┘
┌─ TEST PROGRESS ──────────────────────────────────────────────────┐
│                                                                    │
│  [████████████████████████████░░░░░░░░░░] 75%                    │
└────────────────────────────────────────────────────────────────────┘
┌─ CONTROLS ───────────────────────────────────────────────────────┐
│  [SPACE] Run Test  [Q] Quit  [R] Reset                            │
└────────────────────────────────────────────────────────────────────┘
Server: 1.1.1.1                            2025-11-06 14:32:15
```

## 📥 Installation

### Quick Install

```bash
git clone https://github.com/0xb0rn3/lintspeed.git
cd lintspeed
chmod +x lintspeed
./lintspeed
```

On first run, lintspeed will:
1. 🔍 Auto-detect your Linux distribution
2. 📦 Install Python3 and required dependencies
3. 💾 Ask if you want system-wide installation
4. 🚀 Launch the speed test tool

### Manual Installation

```bash
wget https://raw.githubusercontent.com/0xb0rn3/lintspeed/main/lintspeed
chmod +x lintspeed
sudo mv lintspeed /usr/local/bin/
lintspeed
```

## 🎮 Usage

### Run Speed Test
```bash
lintspeed
```

### Controls
- **SPACE** - Start network speed test
- **R** - Reset all measurements
- **Q** - Quit application

## 🐧 Supported Linux Distributions

- ✅ Debian / Ubuntu
- ✅ Fedora / RHEL / CentOS
- ✅ Arch Linux / Manjaro
- ✅ openSUSE
- ✅ Alpine Linux
- ✅ Any distro with Python 3.6+

## 📊 Metrics Explained

| Metric | Description |
|--------|-------------|
| **Download** | Maximum download speed in Mbps using multiple test files |
| **Upload** | Maximum upload speed in Mbps using varying data sizes |
| **Ping** | Average TCP connection latency to test server |
| **Jitter** | Standard deviation of ping times (network stability) |
| **Packet Loss** | Percentage of failed connection attempts |

## 🔧 Technical Details

- **Language**: Python 3.6+ with Bash wrapper
- **Dependencies**: `requests` library (auto-installed)
- **Testing Method**: Multi-threaded concurrent measurements
- **Ping Samples**: 20 connection attempts for accuracy
- **Download Tests**: 100KB, 1MB, 10MB file transfers
- **Upload Tests**: 100KB, 500KB, 1MB data uploads

## 🛠️ Troubleshooting

### Permission Denied
```bash
chmod +x lintspeed
```

### Python Not Found
```bash
sudo apt install python3 python3-pip  # Debian/Ubuntu
sudo dnf install python3 python3-pip  # Fedora
sudo pacman -S python python-pip      # Arch
```

### Dependency Issues
```bash
pip3 install --break-system-packages requests
```

## 📝 License

MIT License - see [LICENSE](LICENSE) file for details

## 👤 Author

**0xbv1 | 0xb0rn3**

- GitHub: [@0xb0rn3](https://github.com/0xb0rn3)
- Repository: [lintspeed](https://github.com/0xb0rn3/lintspeed)

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## ⭐ Show Your Support

Give a ⭐️ if this project helped you!

## 📜 Changelog

### v1.0 (2025-11-06)
- Initial release
- Beautiful TUI with color-coded metrics
- Auto-detection and dependency installation
- Multi-threaded speed testing
- Packet loss and jitter measurements
- System-wide installation support

---

**Made with ❤️ for the Linux community**
