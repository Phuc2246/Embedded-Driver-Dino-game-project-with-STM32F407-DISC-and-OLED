@echo off
REM STM Dino Game - Windows Virtual Environment Setup Script
REM This script creates a virtual environment and installs all required dependencies

echo ================================
echo STM Dino Game - Windows Setup
echo ================================

REM Navigate to project root
cd /d "%~dp0\.."

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: Python is not installed or not in PATH
    echo Please install Python 3.9+ from python.org
    echo Make sure to check "Add Python to PATH" during installation
    pause
    exit /b 1
)

echo Python version:
python --version

REM Create virtual environment in Windows folder
echo.
echo Creating virtual environment in Windows folder...
cd /d "%~dp0"
if exist "venv" (
    echo Virtual environment already exists. Removing old one...
    rmdir /s /q venv
)

python -m venv venv
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to create virtual environment
    pause
    exit /b 1
)

echo Virtual environment created successfully!

REM Activate virtual environment
echo.
echo Activating virtual environment...
call venv\Scripts\activate.bat

REM Upgrade pip
echo.
echo Upgrading pip...
python -m pip install --upgrade pip

REM Install required packages
echo.
echo Installing required packages...
echo Installing pygame...
pip install pygame==2.6.1
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to install pygame
    pause
    exit /b 1
)

echo Installing pyserial...
pip install pyserial==3.5
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to install pyserial
    pause
    exit /b 1
)

REM Verify installation
echo.
echo Verifying installation...
echo Installed packages:
pip list | findstr -i "pygame pyserial"

echo.
echo ================================
echo Setup completed successfully!
echo ================================
echo.
echo To activate the virtual environment in the future:
echo   venv\Scripts\activate.bat
echo.
echo To run the game:
echo   cd development\src
echo   python StmDinoGame.py
echo.
echo To deactivate the virtual environment:
echo   deactivate
echo.
echo Virtual environment is now ACTIVE in this terminal.
echo You can now run the game!

echo.
echo Virtual environment created at: %cd%\venv
echo Game location: %~dp0..\development\src\StmDinoGame.py

REM Keep the window open so user can see the results
pause
