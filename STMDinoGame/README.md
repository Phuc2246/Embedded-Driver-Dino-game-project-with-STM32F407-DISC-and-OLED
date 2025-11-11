# STM Dino Game

A Chrome Dino-inspired game with serial communication support.

[![Python](https://img.shields.io/badge/Python-3.9+-blue.svg)](https://python.org)
[![Cross Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg)](https://github.com)

## Features

- **Dual Control**: Keyboard (SPACE) or serial commands ('J\n')
- **Serial Communication**: Real-time bidirectional data exchange
- **Auto Platform Detection**: Windows COM ports / Linux USB ports
- **Easy Setup**: One-click virtual environment scripts
- **Jump Analytics**: Real-time height feedback to microcontroller

## Quick Start

### Windows

**Option 1: Run Executable (Easiest)**
```
Double-click STMDinoGame.exe in Windows folder
```

**Option 2: Run from Source**
```cmd
# Note: Requires Python 3.9+

# Navigate to Windows folder
cd .\Windows

# One-time setup (creates virtual environment)
.\setup_venv_windows.bat

# Run game anytime
.\run_game_windows.bat
```

### Linux

```bash
# Note: Requires Python 3.9+

# Add user to dialout group (for serial port access)
sudo usermod -a -G dialout $USER

# Navigate to Linux folder
cd Linux

# Make scripts executable (one-time only)
chmod +x *.sh

# One-time setup (creates virtual environment)
./setup_venv_linux.sh

# Run game anytime
./run_game_linux.sh
```

**Note**: After adding user to dialout group, logout and login for changes to take effect.

## Serial Protocol

| Direction | Data | Description |
|-----------|------|-------------|
| **MCU → Game** | `'J\n'` | Trigger jump |
| **Game → MCU** | `0-100` | Jump height % |

## Troubleshooting

| Issue | Solution |
|-------|----------|
| **Setup script fails** | Check Python installation, run as admin (Windows) |
| **Port not detected** | Install drivers, check Device Manager |
| **Permission denied (Linux)** | `sudo usermod -a -G dialout $USER` |
| **Game won't start** | Re-run the venv setup script, check console for errors |

### System-Specific Issues
#### Windows
```bash
# If antivirus blocks executable:
# Add exception for STMDinoGame.exe in antivirus settings

# If Windows blocks executable from Unknown publisher:
# Just click 'More info' then 'Run anyway', this is a DIY project

# If missing Visual C++ redistributables:
# Download from Microsoft: VC_redist.x64.exe

# If COM port not detected:
# Install device drivers from Device Manager
```

#### Linux
```bash
# If missing X11 libraries:
sudo apt install libx11-6 libxext6

# If audio system missing:
sudo apt install pulseaudio alsa-utils

# If permission issues:
sudo usermod -a -G dialout $USER
# Replace $USER with your username (or just use $USER directly if you’re logged in as yourself): Ex. sudo usermod -a -G dialout tran-nguyen
```
## Support

- **Email**: [Contact the developer](mailto:phucdep1237@gmail.com)

---

**Happy Learning and Gaming!**

*Transform your embedded development journey from blinking LEDs to interactive gameplay!*