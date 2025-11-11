#!/bin/bash

# STM Dino Game - Linux Virtual Environment Runner
# This script activates the virtual environment, runs the game, and deactivates when done

echo "================================"
echo "   STM Dino Game - Linux"
echo "================================"
echo

# Color codes for better output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Start in Linux folder
cd "$(dirname "$0")"

# Check if virtual environment exists in Linux folder
if [ ! -f "venv/bin/activate" ]; then
    print_error "Virtual environment not found in Linux folder!"
    echo "Please run Linux/setup_venv_linux.sh first to create the virtual environment."
    echo
    read -p "Press Enter to exit..."
    exit 1
fi

# Check if game file exists
if [ ! -f "../development/src/StmDinoGame.py" ]; then
    print_error "Game file not found at ../development/src/StmDinoGame.py"
    echo "Please make sure you're in the correct project directory."
    echo
    read -p "Press Enter to exit..."
    exit 1
fi

print_status "Activating virtual environment..."
source venv/bin/activate

echo
echo "================================================"
echo "                 GAME CONTROLS"
echo "================================================"
echo "  KEYBOARD CONTROLS:"
echo "    SPACE BAR - Jump / Start Game"
echo
echo "  SERIAL CONTROLS:"
echo "    Send 'J\\n' - Triggers dino jump"
echo "    Receive    - Jump height percentage (0-100)"
echo
echo "  GAME MECHANICS:"
echo "    - Avoid cacti by jumping over them"
echo "    - Score increases for each obstacle cleared"
echo "    - Game speed increases every 5 points"
echo "    - Press SPACE or close window to restart after game over"
echo
echo "  SERIAL SETUP:"
echo "    1. Configure COM port and baud rate in game UI"
echo "    2. Click 'Open' to establish connection"
echo "    3. Send 'J\\n' from your microcontroller to jump"
echo "================================================"
echo

print_status "Starting STM Dino Game..."
echo "Press Ctrl+C or close the game window to exit."
echo

# Change to game directory and run the game
cd ../development/src
python3 StmDinoGame.py

# Return to Linux folder
cd ../../Linux

echo
print_status "Game session ended."
print_status "Deactivating virtual environment..."
deactivate

print_status "Virtual environment deactivated."
exit 0
