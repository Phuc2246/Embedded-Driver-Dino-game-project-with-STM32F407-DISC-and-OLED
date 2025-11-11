#!/bin/bash

# STM Dino Game - Linux Virtual Environment Setup Script
# This script creates a virtual environment and installs all required dependencies

echo "================================"
echo "STM Dino Game - Linux Setup"
echo "================================"

# Color codes for better output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Navigate to Linux folder to create venv there
cd "$(dirname "$0")"

# Check if Python 3 is installed
if ! command -v python3 &> /dev/null; then
    print_error "Python 3 is not installed"
    echo "Please install Python 3.9+ using your package manager:"
    echo "  Ubuntu/Debian: sudo apt install python3 python3-pip python3-venv"
    echo "  Fedora/RHEL:   sudo dnf install python3 python3-pip"
    echo "  Arch Linux:    sudo pacman -S python python-pip"
    exit 1
fi

print_status "Python version:"
python3 --version

# Check if venv module is available
if ! python3 -m venv --help &> /dev/null; then
    print_error "Python venv module is not available"
    echo "Please install python3-venv:"
    echo "  Ubuntu/Debian: sudo apt install python3-venv"
    echo "  Fedora/RHEL:   sudo dnf install python3-venv"
    exit 1
fi

# Check for system dependencies
print_status "Checking system dependencies..."
missing_deps=()

# Check for development headers and SDL2 libraries
if ! dpkg -l | grep -q python3-dev 2>/dev/null && ! rpm -qa | grep -q python3-devel 2>/dev/null && ! pacman -Q python 2>/dev/null; then
    missing_deps+=("python3-dev/python3-devel")
fi

if ! pkg-config --exists sdl2 2>/dev/null; then
    missing_deps+=("SDL2 development libraries")
fi

if [ ${#missing_deps[@]} -gt 0 ]; then
    print_warning "Some system dependencies might be missing:"
    for dep in "${missing_deps[@]}"; do
        echo "  - $dep"
    done
    echo
    echo "Install system dependencies with:"
    echo "  Ubuntu/Debian:"
    echo "    sudo apt install python3-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
    echo "    sudo apt install libfreetype6-dev libportmidi-dev libjpeg-dev python3-setuptools python3-dev python3-numpy"
    echo
    echo "  Fedora/RHEL:"
    echo "    sudo dnf install python3-devel SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel"
    echo "    sudo dnf install freetype-devel portmidi-devel libjpeg-devel"
    echo
    echo "  Arch Linux:"
    echo "    sudo pacman -S python python-pip sdl2 sdl2_image sdl2_mixer sdl2_ttf"
    echo "    sudo pacman -S freetype2 portmidi libjpeg-turbo"
    echo
fi

# Create virtual environment
print_status "Creating virtual environment..."
if [ -d "venv" ]; then
    print_status "Virtual environment already exists. Removing old one..."
    rm -rf venv
fi

python3 -m venv venv
if [ $? -ne 0 ]; then
    print_error "Failed to create virtual environment"
    exit 1
fi

print_status "Virtual environment created successfully!"

# Activate virtual environment
print_status "Activating virtual environment..."
source venv/bin/activate

# Upgrade pip
print_status "Upgrading pip..."
python -m pip install --upgrade pip

# Install required packages
echo
print_status "Installing required packages..."
print_status "Installing pygame..."
pip install pygame==2.6.1
if [ $? -ne 0 ]; then
    print_error "Failed to install pygame"
    echo "Make sure you have installed the system dependencies listed above"
    exit 1
fi

print_status "Installing pyserial..."
pip install pyserial==3.5
if [ $? -ne 0 ]; then
    print_error "Failed to install pyserial"
    exit 1
fi

# Verify installation
echo
print_status "Verifying installation..."
echo "Installed packages:"
pip list | grep -E "(pygame|pyserial)"

# Check for potential issues
echo
print_status "Checking for potential issues..."

# Check if user is in dialout group (for serial communication)
if ! groups | grep -q dialout; then
    print_warning "User is not in 'dialout' group"
    echo "  For serial communication, you may need to add your user to dialout group:"
    echo "  sudo usermod -a -G dialout \$USER"
    echo "  Then log out and log back in for changes to take effect"
fi

# Test pygame import
if python -c "import pygame; print('pygame version:', pygame.version.ver)" 2>/dev/null; then
    print_status "pygame is working correctly"
else
    print_warning "pygame import test failed"
fi

# Test pyserial import
if python -c "import serial; print('pyserial version:', serial.VERSION)" 2>/dev/null; then
    print_status "pyserial is working correctly"
else
    print_warning "pyserial import test failed"
fi

echo
echo "================================"
echo "Setup completed successfully!"
echo "================================"
echo
print_status "Virtual environment created in: $(pwd)/venv"
echo
echo "To activate the virtual environment in the future:"
echo "  source venv/bin/activate"
echo
echo "To run the game:"
echo "  cd ../development/src"
echo "  python3 StmDinoGame.py"
echo
echo "To deactivate the virtual environment:"
echo "  deactivate"
echo
print_status "Virtual environment is now ACTIVE in this terminal."
print_status "You can now run the game!"
echo
print_status "Virtual environment created at: $(pwd)/venv"
print_status "Game location: $(pwd)/../development/src/StmDinoGame.py"

echo
read -p "Press Enter to continue..."
