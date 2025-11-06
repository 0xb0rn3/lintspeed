<div align="center">

```
╔══════════════════════════════════════════════════════════════════════╗
║                                                                      ║
║    ██╗     ██╗███╗   ██╗████████╗███████╗██████╗ ███████╗███████╗██╗║
║    ██║     ██║████╗  ██║╚══██╔══╝██╔════╝██╔══██╗██╔════╝██╔════╝██║║
║    ██║     ██║██╔██╗ ██║   ██║   ███████╗██████╔╝█████╗  █████╗  ██║║
║    ██║     ██║██║╚██╗██║   ██║   ╚════██║██╔═══╝ ██╔══╝  ██╔══╝  ██║║
║    ███████╗██║██║ ╚████║   ██║   ███████║██║     ███████╗███████╗██║║
║    ╚══════╝╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝╚═╝     ╚══════╝╚══════╝╚═╝║
║                                                                      ║
║              Lightning-Fast Network Performance Testing              ║
║                     by 0xbv1 | 0xb0rn3                              ║
║                                                                      ║
╚══════════════════════════════════════════════════════════════════════╝
```

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Linux-orange.svg?style=for-the-badge&logo=linux)](https://www.linux.org/)
[![Python](https://img.shields.io/badge/Python-3.6+-green.svg?style=for-the-badge&logo=python)](https://www.python.org/)
[![Status](https://img.shields.io/badge/Status-Active-success.svg?style=for-the-badge)](https://github.com/0xb0rn3/lintspeed)

**🚀 Blazing Fast • 🎯 Precise Metrics • 🎨 Beautiful TUI • ⚡ Zero Config**

---

### 📊 Real-Time Network Intelligence at Your Fingertips

</div>

## ✨ Core Features

<table>
<tr>
<td width="50%">

### 🎯 **Precision Testing**
- TCP-based ping measurements (20 samples)
- Multi-endpoint download testing
- Progressive upload benchmarking
- Real-time jitter calculation
- Packet loss detection

</td>
<td width="50%">

### 🎨 **Elite UX**
- Animated progress visualization
- Live updating statistics
- Clean ASCII box-drawing UI
- Status indicators with symbols
- Centered, professional layout

</td>
</tr>
<tr>
<td width="50%">

### ⚡ **Smart Automation**
- First-run auto-setup wizard
- Universal distro detection
- Automatic dependency resolution
- Optional system-wide installation
- Self-contained single-file design

</td>
<td width="50%">

### 🔧 **Technical Excellence**
- Embedded Python in Bash wrapper
- Multi-threaded test execution
- Daemon thread management
- Raw terminal input handling
- Sub-200ms UI refresh rate

</td>
</tr>
</table>

---

## 🖼️ Interface Preview

```
════════════════════════════════════════════════════════════════════════════════
                                 LINTSPEED v1.0
                                by 0xbv1 | 0xb0rn3
════════════════════════════════════════════════════════════════════════════════

────────────────────────────────────────────────────────────────────────────────
                              NETWORK STATISTICS
────────────────────────────────────────────────────────────────────────────────

  Download Speed:    156.42 Mbps     
  Upload Speed:      89.76 Mbps      
  Ping:              12.45 ms        
  Jitter:            2.34 ms         
  Packet Loss:       0.0% ✓          

────────────────────────────────────────────────────────────────────────────────
                               TEST PROGRESS
────────────────────────────────────────────────────────────────────────────────

  [██████████████████████████████████████░░░░░░░░░░░░░░░░░░░░] 68%
  Status: Testing download... 2/3

────────────────────────────────────────────────────────────────────────────────
                                  CONTROLS
────────────────────────────────────────────────────────────────────────────────

                     [ENTER] Run Test  |  [R] Reset  |  [Q] Quit

════════════════════════════════════════════════════════════════════════════════
Server: 1.1.1.1                                       2025-11-06 14:32:15
```

---

## 🚀 Installation

### ⚡ Quick Start (Recommended)

```bash
# Clone and execute
git clone https://github.com/0xb0rn3/lintspeed.git
cd lintspeed
chmod +x lintspeed
./lintspeed
```

**First-run automatic setup will:**
- 🔍 Detect your Linux distribution (Debian/Ubuntu/Fedora/Arch/SUSE/Alpine)
- 📦 Install Python 3 + pip (if needed)
- 🐍 Install `requests` library
- 💾 Offer optional system-wide installation to `/usr/local/bin/`
- 🎯 Launch the speed test interface

---

### 🌐 System-Wide Installation

```bash
# Direct download and install
curl -sSL https://raw.githubusercontent.com/0xb0rn3/lintspeed/main/lintspeed -o lintspeed
chmod +x lintspeed
sudo mv lintspeed /usr/local/bin/
lintspeed
```

Now run `lintspeed` from anywhere in your terminal!

---

## 🎮 Usage Guide

### Starting the Tool

```bash
lintspeed
```

### Interactive Controls

| Key | Action | Description |
|:---:|:------:|:------------|
| **ENTER** | Run Test | Initiates full network diagnostic (ping → download → upload) |
| **R** | Reset | Clears all measurements and returns to ready state |
| **Q** | Quit | Gracefully exits the application |

### Test Sequence

```
Phase 1: Ping Analysis    [░░░░░░░░░░░░░░░░░░░░] 0-25%
         ↓ 20 TCP connection attempts to 1.1.1.1
         
Phase 2: Download Speed   [░░░░░░░░░░░░░░░░░░░░] 25-62.5%
         ↓ 100KB → 1MB → 10MB file transfers
         
Phase 3: Upload Speed     [░░░░░░░░░░░░░░░░░░░░] 62.5-100%
         ↓ 100KB → 500KB → 1MB data uploads
```

---

## 📊 Metrics Breakdown

<table>
<tr>
<th width="20%">Metric</th>
<th width="40%">Measurement Method</th>
<th width="40%">Interpretation</th>
</tr>
<tr>
<td><strong>Download Speed</strong></td>
<td>Maximum throughput from 3 progressive file downloads (100KB, 1MB, 10MB) from <code>speedtest.ftp.otenet.gr</code></td>
<td>Your peak inbound data transfer rate in Mbps</td>
</tr>
<tr>
<td><strong>Upload Speed</strong></td>
<td>Maximum throughput from 3 POST requests with varying payloads (100KB, 500KB, 1MB) to <code>httpbin.org</code></td>
<td>Your peak outbound data transfer rate in Mbps</td>
</tr>
<tr>
<td><strong>Ping</strong></td>
<td>Mean TCP connection time to 1.1.1.1:80 over 20 samples with 2-second timeout</td>
<td>Average network latency; <20ms excellent, 20-50ms good, >100ms poor</td>
</tr>
<tr>
<td><strong>Jitter</strong></td>
<td>Standard deviation of ping measurements (requires ≥2 successful samples)</td>
<td>Network stability; <10ms stable, 10-30ms variable, >30ms unstable</td>
</tr>
<tr>
<td><strong>Packet Loss</strong></td>
<td>Percentage of failed TCP connection attempts out of 20 total</td>
<td>Connection reliability; 0% perfect, <5% acceptable, >5% problematic ⚠</td>
</tr>
</table>

---

## 🐧 Supported Distributions

<div align="center">

| Distribution | Package Manager | Status |
|:------------:|:---------------:|:------:|
| **Debian / Ubuntu / Mint** | `apt-get` | ✅ Tested |
| **Fedora / RHEL / CentOS** | `dnf` / `yum` | ✅ Tested |
| **Arch / Manjaro / EndeavourOS** | `pacman` | ✅ Tested |
| **openSUSE / SLES** | `zypper` | ✅ Tested |
| **Alpine Linux** | `apk` | ✅ Tested |
| **Any Linux with Python 3.6+** | Manual | ✅ Compatible |

</div>

---

## 🔧 Technical Architecture

### Stack Components

```
┌─────────────────────────────────────────────────────────┐
│  Bash Wrapper                                           │
│  • First-run setup wizard                              │
│  • Dependency installation                             │
│  • System-wide deployment option                       │
└────────────────────┬────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│  Embedded Python 3.6+ Core                             │
│  ├─ NetworkTester Class (main engine)                  │
│  ├─ Multi-threaded test execution                      │
│  ├─ Raw terminal input (termios + tty)                 │
│  ├─ Real-time UI rendering loop                        │
│  └─ Exception handling & graceful shutdown             │
└─────────────────────────────────────────────────────────┘
```

### Dependencies

- **Runtime**: Python 3.6+ (auto-installed)
- **Libraries**: `requests` (auto-installed with `--break-system-packages` fallback)
- **System**: Standard GNU/Linux utilities (`clear`, `termios`, `tty`)

### Test Endpoints

| Purpose | Endpoint | Protocol |
|---------|----------|----------|
| Ping | `1.1.1.1:80` | TCP (Cloudflare DNS) |
| Download | `speedtest.ftp.otenet.gr/files/` | HTTP/1.1 |
| Upload | `httpbin.org/post` | HTTP POST |

### Performance Characteristics

- **Test Duration**: ~15-30 seconds (network dependent)
- **UI Refresh Rate**: 200ms (5 FPS)
- **Thread Model**: Daemon background thread for tests
- **Timeout Values**: 2s (ping), 15s (download/upload)
- **Memory Footprint**: <50MB typical

---

## 🛠️ Troubleshooting

<details>
<summary><strong>❌ Permission Denied Error</strong></summary>

```bash
chmod +x lintspeed
```

If system-wide installation fails:
```bash
sudo cp lintspeed /usr/local/bin/
sudo chmod +x /usr/local/bin/lintspeed
```

</details>

<details>
<summary><strong>🐍 Python Not Found</strong></summary>

Manual installation by distro:

```bash
# Debian/Ubuntu
sudo apt update && sudo apt install -y python3 python3-pip

# Fedora
sudo dnf install -y python3 python3-pip

# Arch
sudo pacman -Sy python python-pip

# openSUSE
sudo zypper install -y python3 python3-pip

# Alpine
sudo apk add python3 py3-pip
```

</details>

<details>
<summary><strong>📦 Requests Library Issues</strong></summary>

```bash
# Try with system packages override
pip3 install --break-system-packages requests

# Or use virtual environment
python3 -m venv ~/lintspeed-env
source ~/lintspeed-env/bin/activate
pip install requests
```

</details>

<details>
<summary><strong>🔌 Network Timeout Errors</strong></summary>

- Verify internet connectivity: `ping 1.1.1.1`
- Check firewall rules (TCP port 80)
- Test with different DNS: Edit script to change `self.server = "8.8.8.8"`
- Increase timeout values in code if on slow connection

</details>

<details>
<summary><strong>🎨 UI Rendering Issues</strong></summary>

Ensure terminal supports:
- UTF-8 encoding
- Box-drawing characters (─ ═ │ ║)
- ANSI escape codes
- Minimum 80-column width

Test with: `echo -e "\e[1;32m✓\e[0m UTF-8 support OK"`

</details>

---

## 🤝 Contributing

We welcome contributions from the cybersecurity and network engineering community!

### Development Setup

```bash
git clone https://github.com/0xb0rn3/lintspeed.git
cd lintspeed

# Test locally
./lintspeed

# Make changes to embedded Python (line 75+)
vim lintspeed
```

### Contribution Workflow

1. **Fork** the repository
2. **Create** feature branch: `git checkout -b feature/enhanced-metrics`
3. **Commit** changes: `git commit -m 'Add ICMP ping support'`
4. **Push** to branch: `git push origin feature/enhanced-metrics`
5. **Submit** Pull Request with detailed description

### Ideas for Enhancement

- [ ] ICMP ping option (requires root/capabilities)
- [ ] Multiple server selection
- [ ] Export results to JSON/CSV
- [ ] Historical data tracking
- [ ] IPv6 support
- [ ] HTTP/2 and HTTP/3 testing
- [ ] WebSocket latency measurement
- [ ] Bandwidth throttling detection

---

## 📜 Version History

### **v1.0** (2025-11-06) - Initial Release

**✨ Features**
- Complete network diagnostic suite (ping, jitter, packet loss, download, upload)
- Beautiful terminal UI with ASCII box-drawing and progress bars
- First-run setup wizard with automatic dependency management
- Multi-distribution support (apt, dnf, yum, pacman, zypper, apk)
- Embedded Python architecture in self-contained bash wrapper
- Real-time status updates with 200ms refresh rate
- Keyboard controls (Enter/R/Q) with raw terminal input
- Graceful error handling and connection timeouts

**🔧 Technical**
- TCP-based connectivity testing (20 samples)
- Progressive download testing (100KB → 1MB → 10MB)
- Variable upload benchmarking (100KB → 500KB → 1MB)
- Statistical analysis (mean, standard deviation)
- Multi-threaded test execution with daemon threads

---

## 📄 License

```
MIT License

Copyright (c) 2025 0xb0rn3

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 👤 Author

<div align="center">

**0xbv1 | 0xb0rn3**

Cybersecurity Tool Developer • Red Teamer • Network Engineer

[![GitHub](https://img.shields.io/badge/GitHub-0xb0rn3-black?style=for-the-badge&logo=github)](https://github.com/0xb0rn3)
[![Repository](https://img.shields.io/badge/Repo-lintspeed-blue?style=for-the-badge&logo=git)](https://github.com/0xb0rn3/lintspeed)

</div>

---

## ⭐ Support

If **lintspeed** enhanced your network diagnostics workflow, show your appreciation:

- ⭐ **Star** this repository
- 🐛 **Report** bugs via Issues
- 💡 **Suggest** features
- 🔀 **Fork** and contribute
- 📢 **Share** with your network engineering community

---

<div align="center">

```
╔══════════════════════════════════════════════════════════════╗
║                                                              ║
║    Made with ❤️ for the Linux & Cybersecurity Community     ║
║                                                              ║
║         "Speed is not just a metric, it's a weapon"         ║
║                                                              ║
╚══════════════════════════════════════════════════════════════╝
```

**[Documentation](https://github.com/0xb0rn3/lintspeed/wiki)** • 
**[Issues](https://github.com/0xb0rn3/lintspeed/issues)** • 
**[Releases](https://github.com/0xb0rn3/lintspeed/releases)**

</div>
