@echo off
REM STM Dino Game - Windows Virtual Environment Runner
REM This script activates the virtual environment, runs the game, and deactivates when done

echo ================================
echo   STM Dino Game - Windows
echo ================================
echo.

REM Start in Windows folder
cd /d "%~dp0"

REM Check if virtual environment exists in Windows folder
if not exist "venv\Scripts\activate.bat" (
    echo ERROR: Virtual environment not found in Windows folder!
    echo Please run setup_venv_windows.bat first to create the virtual environment.
    echo.
    pause
    exit /b 1
)

REM Check if game file exists
if not exist "..\development\src\StmDinoGame.py" (
    echo ERROR: Game file not found at ..\development\src\StmDinoGame.py
    echo Please make sure you're in the correct project directory.
    echo.
    pause
    exit /b 1
)

echo Activating virtual environment...
call venv\Scripts\activate.bat

echo.
echo ================================================
echo                 GAME CONTROLS
echo ================================================
echo  KEYBOARD CONTROLS:
echo    SPACE BAR - Jump / Start Game
echo.
echo  SERIAL CONTROLS:
echo    Send 'J\n' - Triggers dino jump
echo    Receive    - Jump height percentage (0-100)
echo.
echo  GAME MECHANICS:
echo    - Avoid cacti by jumping over them
echo    - Score increases for each obstacle cleared
echo    - Game speed increases every 5 points
echo    - Press SPACE or close window to restart after game over
echo.
echo  SERIAL SETUP:
echo    1. Configure COM port and baud rate in game UI
echo    2. Click 'Open' to establish connection
echo    3. Send 'J\n' from your microcontroller to jump
echo ================================================
echo.

echo Starting STM Dino Game...
echo Press Ctrl+C or close the game window to exit.
echo.

REM Change to game directory and run the game
cd ..\development\src
python StmDinoGame.py

REM Return to Windows folder
cd ..\..\Windows.

echo.
echo Game session ended.
echo Deactivating virtual environment...
call deactivate

echo Virtual environment deactivated.
exit
