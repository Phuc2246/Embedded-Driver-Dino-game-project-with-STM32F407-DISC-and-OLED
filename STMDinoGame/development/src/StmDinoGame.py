"""
STM Dino Game - A Chrome Dino-inspired game with STM32 serial communication support
Features:
- Classic jumping dinosaur gameplay
- Serial communication for external control (STM32)
- Configurable COM port and baud rate
- Progressive difficulty scaling
- Real-time jump height feedback via serial
"""

import time
import pygame      # Game graphics and input handling
import sys         # System-specific parameters and functions
import random      # Random number generation for obstacles and clouds
import serial      # Serial communication with STM32
import threading   # Multi-threading for serial communication
import os          # Operating system interface for file operations

# ======================= SCREEN CONFIGURATION =======================
SCREEN_WIDTH = 800    # Game window width in pixels
SCREEN_HEIGHT = 400   # Game window height in pixels

# ======================= GAME MECHANICS SETTINGS =======================
FPS                     = 60    # Frames per second - controls game speed
DINO_ANIMATION_SPEED    = 5     # Animation speed for dino running frames
DINO_JUMP_VELOCITY      = 12    # Initial upward velocity when dino jumps
GRAVITY                 = 0.6   # Gravity force pulling dino down
ON_THE_GROUND           = SCREEN_HEIGHT - 50 - 20  # Ground level Y position
OBJECT_SPEED            = 4     # Initial speed of moving objects (obstacles, ground)

# ======================= COLOR DEFINITIONS =======================
WHITE       = (255, 255, 255)   # Background color
GRAY        = (200, 200, 200)   # UI element color (buttons, dropdowns)
DARK_GRAY   = (100, 100, 100)   # Darker UI elements (hover states)
BLACK       = (0, 0, 0)         # Text color

# ======================= GAME STATE CONSTANTS =======================
READY       = 0    # Game is ready to start, showing start screen
STARTED     = 1    # Game has been started (unused in current implementation)
RUNNING     = 2    # Game is actively running
GAME_OVER   = 3    # Game over state, collision detected
RESTART     = 4    # Restart state (unused in current implementation)

# ======================= DINO STATE CONSTANTS =======================
DINO_READY      = 0    # Dino is in ready position (static)
DINO_RUNNING    = 1    # Dino is running (animated)
DINO_JUMP       = 2    # Dino is jumping (physics-based movement)
DINO_STUCK      = 3    # Dino has collided with obstacle

# ======================= COORDINATE INDEX CONSTANTS =======================
X = 0    # X-coordinate index for position arrays
Y = 1    # Y-coordinate index for position arrays

# ======================= SERIAL COMMUNICATION OPTIONS =======================

# Available COM ports for serial communication with STM32
COM_PORT_OPTION_WINDOW = ['COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7', 'COM8', 'COM9', 'COM10']
USB_PORT_OPTION_LINUX = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', '/dev/ttyUSB3', '/dev/ttyUSB4', 
                          '/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyACM2', '/dev/ttyACM3', '/dev/ttyACM4']

# Automatically detect operating system and assign appropriate serial ports
if sys.platform.startswith('win'):
    SERIAL_PORT_OPTION = COM_PORT_OPTION_WINDOW
    # if you want to debug, uncomment the following line
    # print("Windows detected - Using COM ports")
elif sys.platform.startswith('linux'):
    SERIAL_PORT_OPTION = USB_PORT_OPTION_LINUX
    # if you want to debug, uncomment the following line
    # print("Linux detected - Using USB/ACM ports")
else:
    # Default to Windows COM ports for other systems
    SERIAL_PORT_OPTION = COM_PORT_OPTION_WINDOW
    print(f"Unknown OS ({sys.platform}) - Defaulting to COM ports")

# Available baud rates for serial communication
BAUD_RATE_OPTION = ['4800', '9600', '19200', '38400', '57600', '115200']

def resource_path(relative_path):
    """
    Get absolute path to resource, works for both development and PyInstaller executable
    
    This function handles the difference between running from source code and
    running as a PyInstaller-compiled executable. When PyInstaller creates an
    executable, it extracts bundled resources to a temporary directory.
    
    Args:
        relative_path (str): Relative path to the resource file
        
    Returns:
        str: Absolute path to the resource file
    """
    try:
        # PyInstaller creates this temporary folder and extracts bundled files
        base_path = sys._MEIPASS
        # Remove ../ from the beginning since PyInstaller bundles files in a flat structure
        if relative_path.startswith("../"):
            relative_path = relative_path[3:]  # Remove the "../" prefix
    except Exception:
        # Running from source, use current directory as base
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)

def get_settings_directory():
    """
    Get the appropriate directory for saving user settings
    
    Returns a writable directory for storing user preferences:
    - When running as executable: Uses platform-specific user data directory
    - When running from source: Uses local setting folder
    
    Platform-specific locations:
    - Windows: %LOCALAPPDATA%\\STMDinoGame
    - Linux: ~/.local/share/STMDinoGame
    
    Returns:
        str: Path to settings directory
    """
    try:
        # Running as PyInstaller executable - use platform-specific user data directory
        sys._MEIPASS
        
        # Get platform-specific user data directory
        if sys.platform.startswith('win'):
            # Windows: Use LOCALAPPDATA environment variable
            base_dir = os.environ.get('LOCALAPPDATA', os.path.expanduser('~\\AppData\\Local'))
            settings_dir = os.path.join(base_dir, "STMDinoGame")
        else:
            # Linux and other Unix-like systems: Use XDG specification
            # Default to ~/.local/share if XDG_DATA_HOME is not set
            xdg_data_home = os.environ.get('XDG_DATA_HOME', os.path.expanduser('~/.local/share'))
            settings_dir = os.path.join(xdg_data_home, 'STMDinoGame')
        
        os.makedirs(settings_dir, exist_ok=True)
        return settings_dir
        
    except AttributeError:
        # Running from source - use local setting directory
        settings_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "setting")
        os.makedirs(settings_dir, exist_ok=True)
        return settings_dir

# ======================= RESOURCE PATH DEFINITIONS =======================
# Define paths to all game assets using the resource_path function
# This ensures assets are found both in development and in compiled executable
font_path       = resource_path("../fonts/PressStart2P-Regular.ttf")  # Game font
dino_run1_path  = resource_path("../assets/dino_run1.png")            # Dino animation frame 1
dino_run2_path  = resource_path("../assets/dino_run2.png")            # Dino animation frame 2
dino_run3_path  = resource_path("../assets/dino_run3.png")            # Dino animation frame 3
ground_path     = resource_path("../assets/ground.png")               # Ground texture
cactus1_path    = resource_path("../assets/cactus1.png")              # Cactus obstacle type 1
cactus2_path    = resource_path("../assets/cactus2.png")              # Cactus obstacle type 2
cactus3_path    = resource_path("../assets/cactus3.png")              # Cactus obstacle type 3
cloud_path      = resource_path("../assets/cloud.png")                # Background cloud

# ======================= SETTINGS FILE PATHS =======================
# Define paths to settings files using writable directory
# This ensures settings can be saved both in development and compiled executable
settings_dir = get_settings_directory()
set_COM_path    = os.path.join(settings_dir, "selected_COM_option.txt")   # Saved COM port setting
set_BAUD_path   = os.path.join(settings_dir, "selected_BAUD_option.txt")  # Saved baud rate setting

# ======================= PYGAME INITIALIZATION =======================
pygame.init()  # Initialize all pygame modules
font = pygame.font.SysFont(None, 20)         # System font for UI text
font = pygame.font.Font(font_path, 20)       # Custom pixel font for retro look


class Dino:
    """
    Dinosaur character class handling all dino-related functionality
    
    This class manages the dino's appearance, animation, physics, and collision detection.
    The dino has multiple states: ready, running, jumping, and stuck (collision).
    """
    
    def __init__(self):
        """Initialize the dinosaur with default properties and load animation frames"""
        # Physical properties
        self.dino_hight = 50                    # Dino sprite height in pixels
        self.dino_width = 50                    # Dino sprite width in pixels
        self.dino_x = 50                        # Fixed X position (dino doesn't move horizontally)
        self.dino_y = ON_THE_GROUND             # Current Y position
        
        # State management
        self.dino_state = DINO_READY            # Current dino state
        
        # Physics properties
        self.dino_jump_velocity = DINO_JUMP_VELOCITY  # Current vertical velocity
        self.gravity = GRAVITY                        # Gravity constant
        
        # Animation properties
        self.dino_animation_timer = 0           # Timer for animation frame switching
        self.dino_frame_index = 0               # Current animation frame index
        
        # Jump analytics for STM32 communication
        self.dino_jump_max_height = 0           # Maximum jump height (calculated once)
        self.dino_jump_height_perc = 0          # Current jump height as percentage
        
        # Load and scale animation frames
        self.dino_run_frames = [
            pygame.transform.scale(pygame.image.load(dino_run1_path), (self.dino_width, self.dino_hight)),
            pygame.transform.scale(pygame.image.load(dino_run2_path), (self.dino_width, self.dino_hight)),
            pygame.transform.scale(pygame.image.load(dino_run3_path), (self.dino_width, self.dino_hight))
        ]

    def Dino_ready(self, screen):
        """
        Render dino in ready state (static, first frame)
        
        Args:
            screen: Pygame surface to draw on
        """
        screen.blit(self.dino_run_frames[0], (self.dino_x, self.dino_y))
    
    def run_Dino_run(self, screen): #:)))))
        """
        Handle dino running animation by cycling through frames
        
        Args:
            screen: Pygame surface to draw on
        """
        # Increment animation timer
        self.dino_animation_timer += 1
        
        # Switch to next frame when timer reaches threshold
        if self.dino_animation_timer >= DINO_ANIMATION_SPEED:
            self.dino_frame_index = (self.dino_frame_index + 1) % len(self.dino_run_frames)
            self.dino_animation_timer = 0
            
        # Draw current animation frame
        screen.blit(self.dino_run_frames[self.dino_frame_index], (self.dino_x, self.dino_y))

    def jump_Dino_jump(self, screen): #:))
        """
        Handle dino jumping physics and rendering
        
        Applies gravity and velocity to create realistic jumping motion.
        When dino lands, returns to running state.
        
        Args:
            screen: Pygame surface to draw on
        """
        # Apply physics: move dino up by current velocity
        self.dino_y -= self.dino_jump_velocity
        
        # Apply gravity: reduce upward velocity
        self.dino_jump_velocity -= self.gravity
        
        # Check if dino has landed
        if self.dino_y >= ON_THE_GROUND:
            self.dino_y = ON_THE_GROUND                    # Snap to ground level
            self.dino_state = DINO_RUNNING                 # Return to running state
            self.dino_jump_velocity = DINO_JUMP_VELOCITY   # Reset jump velocity
            
        # Draw dino (static frame during jump)
        screen.blit(self.dino_run_frames[0], (self.dino_x, self.dino_y))

    def check_Dino_stuck(self, obstacles):
        """
        Advanced collision detection between dino and obstacles
        
        This collision system is designed for better gameplay experience.
        It uses different collision boundaries depending on dino's height:
        - When jumping high: smaller collision box (more forgiving)
        - When on ground: larger collision box (more realistic)
        
        Args:
            obstacles: Obstacles object containing list of active obstacles
        """
        if not obstacles.obstacles:  # No obstacles to check
            return
            
        obstacle = obstacles.obstacles[0]  # Check collision with first (closest) obstacle
        
        # Adjust collision box based on dino height
        if (self.dino_y < SCREEN_HEIGHT - 20 - (self.dino_hight * 1.2)):
            # Dino is jumping high - use smaller collision box for forgiveness
            xmax = self.dino_width - 40
        else:
            # Dino is low/on ground - use larger collision box for realism
            xmax = self.dino_width - 10
        
        # Check collision on left side of obstacle
        if (self.dino_x + xmax >  obstacle[X]) and \
           (self.dino_x + xmax <  obstacle[X] + (obstacle[3] / 2)) and \
           (obstacle[Y] < self.dino_y + 40):
            self.dino_state = DINO_STUCK
            
        # Check collision in middle of obstacle
        if (self.dino_x > obstacle[X] + (obstacle[3] / 2)) and \
           (self.dino_x <  obstacle[X] + obstacle[3] - xmax) and \
           (obstacle[Y] < self.dino_y + 40):
            self.dino_state = DINO_STUCK
    
    def Init_highest_jumping_position(self):
        """
        Calculate the maximum jump height for percentage calculations
        
        This function simulates the jump physics to find the highest point
        the dino can reach. This is used later to calculate jump height
        percentage for STM32 communication.
        
        Note: Call this only once during initialization, not in game loop!
        """
        dino_y_ex = ON_THE_GROUND                    # Start at ground level
        dino_jump_velocity_ex = self.dino_jump_velocity  # Initial velocity
        
        # Simulate jump physics until peak is reached
        while True:
            dino_y_ex -= dino_jump_velocity_ex       # Move up
            dino_jump_velocity_ex -= self.gravity    # Apply gravity
            
            # Stop when velocity becomes positive (falling down)
            if (dino_y_ex <= dino_y_ex - dino_jump_velocity_ex) and (dino_jump_velocity_ex >= 0):
                self.dino_jump_max_height = dino_y_ex
                break
    
    def Dino_jump_height_perc(self):
        """
        Calculate current jump height as percentage of maximum height
        
        This percentage is sent to STM32 for real-time jump feedback.
        Range: 0% (ground) to 100% (peak of jump)
        """
        if self.dino_jump_max_height != 0:  # Avoid division by zero
            self.dino_jump_height_perc = int(((ON_THE_GROUND - self.dino_y) / self.dino_jump_max_height) * 100)

class Ground:
    """
    Ground/terrain class for creating infinite scrolling ground effect
    
    This class handles the moving ground texture that creates the illusion
    of the dino running forward while actually staying in place.
    """
    
    def __init__(self):
        """Initialize ground properties and load ground texture"""
        self.ground_x = 0                           # Current X position of ground
        self.ground_y = 40                          # Height of ground from bottom
        self.ground_moving_speed = OBJECT_SPEED     # Speed of ground movement
        
        # Load ground texture image
        self.ground_image = pygame.image.load(ground_path)
    
    def draw_ground(self, screen):
        """
        Draw ground texture at current position
        
        Draws two copies of the ground image side by side to create
        seamless scrolling effect.
        
        Args:
            screen: Pygame surface to draw on
        """
        # Draw first ground image
        screen.blit(self.ground_image, (self.ground_x, SCREEN_HEIGHT - self.ground_y))
        # Draw second ground image right after the first one
        screen.blit(self.ground_image, (self.ground_x + self.ground_image.get_width(), SCREEN_HEIGHT - self.ground_y))

    def move_ground(self, screen):
        """
        Update ground position and redraw for scrolling effect
        
        Moves ground to the left and resets position when it scrolls
        completely off screen to create infinite scrolling.
        
        Args:
            screen: Pygame surface to draw on
        """
        # Move ground to the left
        self.ground_x -= self.ground_moving_speed
        
        # Reset position when ground image has scrolled completely off screen
        if self.ground_x <= -self.ground_image.get_width():
            self.ground_x = 0
            
        # Draw the updated ground
        self.draw_ground(screen)

class Obstacles:
    """
    Obstacle management system for cactus obstacles
    
    This class handles spawning, moving, and removing cactus obstacles.
    It manages a list of active obstacles and provides collision detection
    data to the Dino class.
    """
    
    def __init__(self):
        """Initialize obstacle system with different cactus types"""
        # Cactus dimensions (different sizes for variety)
        self.cactus_height  = 50        # Standard height for all cacti
        self.cactus_width_1 = 50        # Small cactus width
        self.cactus_width_2 = 60        # Medium cactus width
        self.cactus_width_3 = 90        # Large cactus width
        
        # Obstacle management
        self.obstacle_spawn_timer = 0   # Timer for spawning new obstacles
        self.obstacle_speed = OBJECT_SPEED  # Speed of obstacle movement
        self.obstacle_removed = False   # Flag indicating if an obstacle was removed (for scoring)
        self.dx = 400                   # Minimum distance between obstacles
        
        # Load and scale different cactus images
        self.cactus_images = [
            pygame.transform.scale(pygame.image.load(cactus1_path), (self.cactus_width_1, self.cactus_height)),
            pygame.transform.scale(pygame.image.load(cactus2_path), (self.cactus_width_2, self.cactus_height)),
            pygame.transform.scale(pygame.image.load(cactus3_path), (self.cactus_width_3, self.cactus_height)),
        ]
        
        # Initialize with first obstacle
        cactus_image = random.choice(self.cactus_images)
        # Obstacle format: [x_position, y_position, image, width]
        self.obstacles = [[SCREEN_WIDTH, ON_THE_GROUND, cactus_image, cactus_image.get_width()]]

    def spawn_obstacle(self):
        """
        Create new obstacles when appropriate spacing is achieved
        
        Spawns a new random cactus when the last obstacle has moved
        far enough from the right edge of the screen.
        """
        # Check if enough distance has passed since last obstacle
        if SCREEN_WIDTH - self.obstacles[-1][X] > self.dx:
            # Create new obstacle at random distance from screen edge
            obstacle_x = SCREEN_WIDTH + random.randrange(0, 400, 200)
            obstacle_y = ON_THE_GROUND
            cactus_image = random.choice(self.cactus_images)  # Random cactus type
            
            # Add new obstacle to list
            self.obstacles.append([obstacle_x, obstacle_y, cactus_image, cactus_image.get_width()])

    def move_obstacle(self):
        """
        Move all active obstacles to the left
        
        Updates X position of all obstacles to create movement effect.
        """
        for obstacle in self.obstacles:
            obstacle[0] -= self.obstacle_speed
    
    def remove_outscreen_obs(self):
        """
        Remove obstacles that have moved completely off screen
        
        Removes obstacles from the list when they're no longer visible
        and sets flag for score increment.
        """
        if self.obstacles:
            # Check if first obstacle (leftmost) is completely off screen
            if self.obstacles[0][0] + self.obstacles[0][3] < 0:
                self.obstacles.remove(self.obstacles[0])
                self.obstacle_removed = True  # Flag for score system

    def draw_obstacles(self, screen):
        """
        Main obstacle management function called each frame
        
        Handles spawning, movement, cleanup, and rendering of all obstacles.
        
        Args:
            screen: Pygame surface to draw on
        """
        self.spawn_obstacle()        # Create new obstacles if needed
        self.move_obstacle()         # Move all obstacles left
        self.remove_outscreen_obs()  # Clean up off-screen obstacles
        
        # Draw all active obstacles
        for obstacle in self.obstacles:
            screen.blit(obstacle[2], (obstacle[0], obstacle[1]))

class Cloud:
    """
    Background cloud system for visual atmosphere
    
    Creates slowly moving clouds in the background to enhance the
    visual experience and add depth to the game scene.
    """
    
    def __init__(self):
        """Initialize cloud system with random positioned clouds"""
        self.cloud_speed = 1  # Slow movement speed for background effect
        
        # Initialize multiple clouds at random positions
        self.clouds = [
            [random.randint(0, SCREEN_WIDTH), SCREEN_HEIGHT // 2 + random.randint(10, 30)],
            [random.randint(0, SCREEN_WIDTH), SCREEN_HEIGHT // 2 + random.randint(60, 90)],
        ]
        
        # Load and scale cloud image
        self.cloud_image = pygame.transform.scale(pygame.image.load(cloud_path), (50, 25))
    
    def move_clouds(self):
        """
        Move all clouds slowly to the left
        
        Creates parallax effect with slower movement than ground/obstacles.
        """
        for cloud in self.clouds:
            cloud[0] -= self.cloud_speed

    def spawn_clouds(self):
        """
        Respawn clouds that have moved off screen
        
        When a cloud exits the left side, it reappears on the right
        side at a random height for continuous cloud coverage.
        """
        for cloud in self.clouds:
            # Check if cloud has moved completely off screen
            if cloud[0] + self.cloud_image.get_width() < 0:
                # Respawn on right side with random positioning
                cloud[0] = SCREEN_WIDTH + random.randint(100, 300)
                cloud[1] = SCREEN_HEIGHT // 2 + random.randint(0, 100)

    def draw_clouds(self, screen):
        """
        Main cloud management function called each frame
        
        Handles movement, respawning, and rendering of all clouds.
        
        Args:
            screen: Pygame surface to draw on
        """
        self.spawn_clouds()  # Respawn off-screen clouds
        self.move_clouds()   # Move all clouds
        
        # Draw all active clouds
        for cloud in self.clouds:
            screen.blit(self.cloud_image, (cloud[0], cloud[1]))

class Serial:
    """
    Serial communication handler for STM32 integration
    
    This class manages bidirectional serial communication:
    - Receives jump commands from external microcontroller
    - Sends real-time jump height data to microcontroller
    - Handles connection management and error recovery
    """
    
    def __init__(self, COM_port=None, baud_rate=None):
        """
        Initialize serial communication parameters
        
        Args:
            COM_port (str): COM port identifier (e.g., 'COM3')
            baud_rate (str): Communication speed (e.g., '115200')
        """
        self.COM_port = COM_port               # Serial port identifier
        self.baud_rate = baud_rate             # Communication baud rate
        self.serial_thread_running = False     # Thread control flag
        self.received_data = ""                # Buffer for incoming data
        self.COM_jump = False                  # Jump command flag from MCU
        self.serial_port = None                # Serial port object
        
        # Thread safety: Lock for COM_jump variable access
        self.com_jump_lock = threading.Lock()  # Prevents race conditions between threads
    
    def open_serial_port(self):
        """
        Establish serial connection and start communication thread
        
        Creates a separate thread for non-blocking serial communication
        to prevent game from freezing during I/O operations.
        """
        try:
            # Attempt to open serial port with specified parameters
            self.serial_port = serial.Serial(self.COM_port, baudrate=self.baud_rate, timeout=1)
            
            if self.serial_port.is_open:
                print(f"Successfully opened {self.COM_port} at {self.baud_rate} baud rate")
                
                # Start background thread for serial communication
                self.serial_thread_running = True
                self.serial_thread = threading.Thread(target=self.run_serial_thread)
                self.serial_thread.daemon = True  # Dies when main thread dies
                self.serial_thread.start()
                
        except serial.SerialException as e:
            print(f"Error opening serial port {self.COM_port}: {e}")
            self.serial_port = None
    
    def close_serial_port(self):
        """
        Safely close serial connection and stop communication thread
        
        Ensures proper cleanup of resources and thread termination.
        """
        if self.serial_port and self.serial_port.is_open:
            # Signal thread to stop
            self.serial_thread_running = False
            
            # Wait for thread to finish
            self.serial_thread.join()
            
            # Close physical serial port
            self.serial_port.close()
            self.serial_port = None
            print(f"Successfully closed {self.COM_port}") 

    def run_serial_thread(self):
        """
        Background thread function for continuous serial communication
        
        Runs in separate thread to handle incoming data without blocking
        the main game loop. Continuously reads and processes serial data.
        """
        try:
            while self.serial_thread_running and self.serial_port and self.serial_port.is_open:
                self.read_from_com_port()      # Check for incoming data
                self.decode_data_from_com_port()  # Process received data
                time.sleep(0.01)  # 10ms delay to prevent CPU overload
        except serial.SerialException as e:
            print(f"Error with serial port {self.COM_port}: {e}")
            self.serial_thread_running = False  # Stop thread on error
            
    def read_from_com_port(self):
        """
        Read incoming data from serial port
        
        Non-blocking read that processes any available data.
        Data is expected to be newline-terminated strings.
        """
        if self.serial_port and self.serial_port.in_waiting > 0:
            # Read one line of data
            data = self.serial_port.readline()
            # Decode and clean the received data
            self.received_data = data.decode('utf-8').strip()

    def decode_data_from_com_port(self):
        """
        Process and interpret received serial data
        
        Current protocol:
        - "J" = Jump command from microcontroller
        - Can be extended for other commands (pause, restart, etc.)
        
        Thread-safe implementation with lock protection for COM_jump.
        """
        if self.received_data == "J":
            # Thread-safe write to COM_jump flag
            with self.com_jump_lock:
                self.COM_jump = True           # Set jump flag for game loop
            self.received_data = ''            # Clear buffer after processing
    
    def send_to_com_port(self, data):
        """
        Send data to the serial port
        
        Transmits data to connected microcontroller. Used primarily
        for sending real-time jump height percentage.
        
        Args:
            data: Data to send (will be converted to string and newline-terminated)
        """
        if self.serial_port and self.serial_port.is_open:
            try:
                # Send data with newline terminator
                self.serial_port.write(f"{data}\n".encode('utf-8'))
            except serial.SerialException as e:
                print(f"Error sending data to serial port {self.COM_port}: {e}")

class Dropdown:
    """
    Dropdown menu UI component for serial port configuration
    
    Provides a user-friendly interface for selecting COM ports and baud rates.
    Includes persistence to save user preferences between game sessions.
    """
    
    def __init__(self, x, y, width, height, options, save_file):
        """
        Initialize dropdown menu with position and options
        
        Args:
            x, y (int): Position of dropdown on screen
            width, height (int): Dimensions of dropdown
            options (list): List of selectable options
            save_file (str): Path to file for saving selected option
        """
        self.rect = pygame.Rect(x, y, width, height)    # Dropdown boundary
        self.options = options                          # Available choices
        self.selected_index = 0                         # Currently selected option index
        self.dropdown_open = False                      # Whether dropdown is expanded
        self.option_height = height                     # Height of each option item
        self.save_file = save_file                      # File path for persistence
        self.edit_block = False                         # Whether editing is blocked (when serial is open)
        self.text_color = BLACK                         # Text color (changes when blocked)
        
        # Load previously saved selection
        self.load_selected()

    def draw(self, surface, font):
        """
        Render the dropdown menu on screen
        
        Draws the selected item and, if open, all available options.
        
        Args:
            surface: Pygame surface to draw on
            font: Font to use for text rendering
        """
        # Draw main dropdown box with selected item
        pygame.draw.rect(surface, GRAY, self.rect)
        text = font.render(self.options[self.selected_index], True, self.text_color)
        surface.blit(text, (self.rect.x + 5, self.rect.y + 5))

        # Draw dropdown options if menu is open
        if self.dropdown_open:
            for i, option in enumerate(self.options):
                # Calculate position for each option
                item_rect = pygame.Rect(
                    self.rect.x,
                    self.rect.y + self.option_height * (i + 1),
                    self.rect.width,
                    self.option_height,
                )
                
                # Draw option background and text
                pygame.draw.rect(surface, DARK_GRAY, item_rect)
                text = font.render(option, True, WHITE)
                surface.blit(text, (item_rect.x + 5, item_rect.y + 5))

    def handle_event(self):
        """
        Process mouse interaction with dropdown menu
        
        Handles clicking on dropdown to open/close and selecting options.
        Only responds to input if editing is not blocked.
        """
        if self.edit_block == False:  # Only handle events if not blocked
            mx, my = pygame.mouse.get_pos()
            
            # Check if main dropdown area was clicked
            if self.rect.collidepoint(mx, my):
                self.dropdown_open = not self.dropdown_open  # Toggle open state
                
            elif self.dropdown_open:
                # Check if any option was clicked
                for i in range(len(self.options)):
                    item_rect = pygame.Rect(
                        self.rect.x,
                        self.rect.y + self.option_height * (i + 1),
                        self.rect.width,
                        self.option_height,
                    )
                    
                    if item_rect.collidepoint(mx, my):
                        self.selected_index = i    # Update selection
                        self.dropdown_open = False # Close dropdown
                        break
                else:
                    # Clicked outside dropdown, close it
                    self.dropdown_open = False

    def save_selected(self):
        """
        Save currently selected option to file for persistence
        
        Stores the selected index so user preference is remembered
        between game sessions.
        """
        with open(self.save_file, "w") as f:
            f.write(str(self.selected_index))

    def load_selected(self):
        """
        Load previously saved selection from file
        
        Restores user's last selection if save file exists and
        contains valid data.
        """
        if os.path.exists(self.save_file):
            with open(self.save_file, "r") as f:
                val = f.read().strip()
                if val.isdigit():
                    idx = int(val)
                    # Validate index is within bounds
                    if 0 <= idx < len(self.options):
                        self.selected_index = idx

class Button:
    """
    Button UI component for serial connection control
    
    Provides a clickable button for opening/closing serial connections
    with visual feedback for user interactions.
    """
    
    def __init__(self, x, y, width, height, text, on_click=None):
        """
        Initialize button with position, appearance, and callback
        
        Args:
            x, y (int): Button position on screen
            width, height (int): Button dimensions
            text (str): Text to display on button
            on_click (function): Callback function when button is clicked
        """
        self.rect = pygame.Rect(x, y, width, height)    # Button boundary
        self.text = text                                # Button label
        self.on_click = on_click                        # Click callback function
        self.color = GRAY                               # Current button color
        self.text_surf = font.render(text, True, WHITE) # Pre-rendered text
        self.is_click = False                           # Whether button is currently pressed

    def draw(self, surface):
        """
        Render button on screen with current state
        
        Args:
            surface: Pygame surface to draw on
        """
        # Draw button background
        pygame.draw.rect(surface, self.color, self.rect)
        
        # Render and center button text
        self.text_surf = font.render(self.text, True, WHITE)
        text_rect = self.text_surf.get_rect(center=self.rect.center)
        surface.blit(self.text_surf, text_rect)

    def handle_event(self, event):
        """
        Process mouse events for button interaction
        
        Handles button press/release visual feedback and executes
        callback function when clicked.
        
        Args:
            event: Pygame event object
        """
        if event.type == pygame.MOUSEBUTTONDOWN:
            if self.rect.collidepoint(event.pos):
                # Button pressed - change appearance and execute callback
                self.color = DARK_GRAY
                self.is_click = True
                if self.on_click:
                    self.on_click()
                    
        if event.type == pygame.MOUSEBUTTONUP:
            # Button released - restore normal appearance
            self.color = GRAY
            self.is_click = False

    def update(self):
        """
        Update button appearance based on mouse hover state
        
        Optional method for hover effects (currently unused in main game loop).
        """
        if self.rect.collidepoint(pygame.mouse.get_pos()):
            self.color = DARK_GRAY  # Hover state
        else:
            self.color = GRAY       # Normal state

class DinoGame:
    """
    Main game class that orchestrates all game components
    
    This is the central controller that manages:
    - Game state transitions (ready, running, game over)
    - Input handling (keyboard, mouse, serial)
    - Game object coordination (dino, obstacles, ground, clouds)
    - UI management (dropdowns, buttons)
    - Serial communication integration
    - Score tracking and difficulty progression
    """
    
    def __init__(self, width=SCREEN_WIDTH, height=SCREEN_HEIGHT):
        """
        Initialize the complete game system
        
        Sets up pygame, creates all game objects, and initializes
        the user interface components.
        
        Args:
            width, height (int): Game window dimensions
        """
        # Game window setup
        self.width = width
        self.height = height
        
        # Game state management
        self.game_state = READY  # Start in ready state
        self.score = 0          # Player's current score
        self.jump_max_height_calculated = False  # Jump max height calculated flag

        # Initialize pygame display
        self.screen = pygame.display.set_mode((self.width, self.height), pygame.HWSURFACE | pygame.DOUBLEBUF)
        pygame.display.set_caption('STM Dino game')
        self.clock = pygame.time.Clock()  # For frame rate control
        
        # Display optimization with dirty rectangles
        self.dirty_rects = []           # List of rectangles that need updating
        self.background_surface = None  # Background surface for efficient clearing
        self.use_dirty_rects = False    # Flag to enable/disable optimization (disabled for now)
        
        # Create game objects
        self.ground = Ground()          # Scrolling ground system
        self.dino = Dino()              # Player character
        self.obstacles = Obstacles()    # Obstacle management
        self.cloud = Cloud()            # Background atmosphere
        
        # Serial communication setup
        self.COM_port = Serial()     # Serial handler object
        
        # User interface components
        TRIMED_SERIAL_PORT_OPTION = [port.replace('/dev/tty', '') for port in SERIAL_PORT_OPTION]
        self.dropdown_COM = Dropdown(SCREEN_WIDTH - 180, 20, 130, 30, TRIMED_SERIAL_PORT_OPTION, set_COM_path)
        self.dropdown_BAUD = Dropdown(SCREEN_WIDTH - 180, 70, 130, 30, BAUD_RATE_OPTION, set_BAUD_path)
        self.button_open_COM = Button(SCREEN_WIDTH - 180, 150, 130, 30, "Open", self.COM_port.open_serial_port)

    def add_dirty_rect(self, rect):
        """
        Add a rectangle to the dirty rectangles list
        
        Dirty rectangles are areas of the screen that need to be redrawn.
        This allows for partial screen updates instead of redrawing everything.
        
        Args:
            rect (pygame.Rect): Rectangle area that needs updating
        """
        if self.use_dirty_rects and rect:
            # Clamp rect to screen boundaries
            clipped_rect = rect.clip(pygame.Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT))
            if clipped_rect.width > 0 and clipped_rect.height > 0:
                self.dirty_rects.append(clipped_rect)
    
    def create_background_surface(self):
        """
        Create a background surface for efficient screen clearing
        
        Pre-renders the static background elements to improve performance
        when clearing and redrawing dynamic elements.
        """
        if not self.background_surface:
            self.background_surface = pygame.Surface((SCREEN_WIDTH, SCREEN_HEIGHT))
            self.background_surface.fill(WHITE)
    
    def clear_dirty_areas(self):
        """
        Clear dirty areas by blitting background to those regions
        
        Instead of clearing the entire screen, only clears areas that changed.
        This significantly reduces the amount of pixels that need to be redrawn.
        """
        if self.use_dirty_rects and self.background_surface:
            for rect in self.dirty_rects:
                self.screen.blit(self.background_surface, rect, rect)

    def display_start_screen(self, screen):
        """
        Show start screen with instructions
        
        Args:
            screen: Pygame surface to draw on
        """
        text = font.render("Press SPACE to start", True, BLACK)
        screen.blit(text, (SCREEN_WIDTH // 2 - 200, SCREEN_HEIGHT // 2))
    
    def display_game_over(self, screen):
        """
        Show game over message
        
        Args:
            screen: Pygame surface to draw on
        """
        text = font.render("Game Over!", True, BLACK)
        screen.blit(text, (SCREEN_WIDTH // 2 - 100, SCREEN_HEIGHT // 2))

    def display_score(self, screen):
        """
        Display current score on screen
        
        Args:
            screen: Pygame surface to draw on
        """
        text = font.render(f"Score: {self.score}", True, BLACK)
        screen.blit(text, (10, 10))
    
    def reset_game(self):
        """
        Reset all game objects and parameters for a new game
        
        Called when starting a new game after game over.
        Reinitializes all dynamic objects and resets score.
        """
        # Destroy and recreate game objects for clean state
        del self.dino
        self.dino = Dino()
        del self.obstacles
        self.obstacles = Obstacles()
        del self.ground
        self.ground = Ground()
        
        # Reset game parameters
        self.game_state = RUNNING
        self.dino.dino_state = DINO_RUNNING
        self.score = 0
        
        # Initialize jump analytics and send initial data to STM32
        self.dino.Init_highest_jumping_position()
        self.COM_port.send_to_com_port(self.dino.dino_jump_height_perc)
    
    def handle_game_state(self):
        """
        Process game state transitions based on current state
        
        Handles the logic for starting the game and making the dino jump
        depending on the current game state.
        """
        if self.game_state == READY:
            # Start the game and make dino jump immediately
            self.game_state = RUNNING
            self.dino.dino_state = DINO_JUMP
        elif self.game_state == RUNNING:
            # Game is running, make dino jump
            self.dino.dino_state = DINO_JUMP
        elif self.game_state == GAME_OVER:
            # Restart the game
            self.reset_game()

    def handle_keypress(self):
        """
        Process all user input events (keyboard, mouse, window)
        
        This is the main input handler that processes:
        - Window close events
        - Spacebar for jumping/starting
        - Mouse clicks for UI interaction
        - Serial port configuration
        """
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                # Clean shutdown when window is closed
                self.dropdown_COM.save_selected()    # Save user preferences
                self.dropdown_BAUD.save_selected()
                self.COM_port.serial_thread_running = False  # Stop serial thread
                
                # Close serial port if open
                if self.COM_port.serial_port and self.COM_port.serial_port.is_open:
                    self.COM_port.close_serial_port()
                    
                pygame.quit()
                sys.exit()
                
            elif event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                # Spacebar pressed - handle jumping/starting
                self.handle_game_state()
                
            elif event.type == pygame.MOUSEBUTTONDOWN:
                # Mouse button pressed - handle UI interactions
                
                # Update dropdown selections
                self.dropdown_COM.handle_event()
                self.COM_port.COM_port = SERIAL_PORT_OPTION[self.dropdown_COM.selected_index]
                
                self.dropdown_BAUD.handle_event()
                self.COM_port.baud_rate = self.dropdown_BAUD.options[self.dropdown_BAUD.selected_index]
                
                # Handle serial connection button
                self.button_open_COM.handle_event(event)
                
                if self.button_open_COM.is_click == True:
                    if self.COM_port.serial_port == None:
                        # Currently disconnected - prepare for connection
                        self.button_open_COM.text = "Open"
                        self.button_open_COM.on_click = self.COM_port.open_serial_port
                        # Enable dropdown editing
                        self.dropdown_COM.edit_block = False
                        self.dropdown_COM.text_color = BLACK
                        self.dropdown_BAUD.edit_block = False
                        self.dropdown_BAUD.text_color = BLACK
                    else:
                        # Currently connected - prepare for disconnection
                        self.button_open_COM.text = "Close"
                        self.button_open_COM.on_click = self.COM_port.close_serial_port
                        # Disable dropdown editing while connected
                        self.dropdown_COM.edit_block = True
                        self.dropdown_COM.text_color = WHITE
                        self.dropdown_BAUD.edit_block = True
                        self.dropdown_BAUD.text_color = WHITE
                        
            elif event.type == pygame.MOUSEBUTTONUP:
                # Mouse button released
                self.button_open_COM.handle_event(event)

    def handle_COM_signal(self):
        """
        Process jump commands received from serial port (Thread-safe)
        
        Checks for jump commands from connected STM32 and
        triggers the same game state handling as keyboard input.
        Uses thread lock to prevent race conditions.
        """
        # Thread-safe read and clear of COM_jump flag
        with self.COM_port.com_jump_lock:
            if self.COM_port.COM_jump == True:
                self.handle_game_state()        # Trigger jump/start
                self.COM_port.COM_jump = False  # Clear the flag

    def run(self):
        """
        Main game loop - the heart of the game
        
        This method contains the primary game loop that:
        1. Maintains consistent frame rate (60 FPS)
        2. Processes user input and serial communication
        3. Updates game state and objects
        4. Renders all visual elements
        5. Handles game state transitions
        
        The loop continues until the user closes the game.
        """
        # Main game loop - runs until game is closed
        while True:
            # Control frame rate to maintain consistent game speed
            self.clock.tick(FPS)
            
            # Process all input sources
            self.handle_keypress()    # Keyboard and mouse input
            self.handle_COM_signal()  # Serial port input from STM32

            if self.game_state == READY:
                """
                READY STATE: Game start screen
                - Show instructions and UI elements
                - Initialize dino jump analytics
                - Display static game elements
                """
                # Calculate jump height ONLY ONCE
                if not self.jump_max_height_calculated:
                    self.dino.Init_highest_jumping_position()  # Calculate max jump height
                    self.jump_max_height_calculated = True
                
                # Optimized screen clearing and background setup
                self.create_background_surface()  # Create background if needed
                if self.use_dirty_rects:
                    self.clear_dirty_areas()  # Clear only changed areas
                else:
                    self.screen.fill(WHITE)   # Fallback to full screen clear
                
                # Draw background elements and track dirty areas
                self.cloud.draw_clouds(self.screen)
                self.ground.draw_ground(self.screen)
                self.dino.Dino_ready(self.screen)
                
                # Add UI areas as dirty rectangles for proper updating
                self.add_dirty_rect(pygame.Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT // 2))  # Text area
                self.add_dirty_rect(pygame.Rect(SCREEN_WIDTH - 200, 0, 200, 250))        # UI area
                
                # Show start instructions and UI
                self.display_start_screen(self.screen)
                self.button_open_COM.draw(self.screen)
                self.dropdown_BAUD.draw(self.screen, font)
                self.dropdown_COM.draw(self.screen, font)

            if self.game_state == RUNNING:
                """
                RUNNING STATE: Active gameplay
                - Update all moving objects
                - Handle collision detection
                - Manage score and difficulty progression
                - Send real-time data to STM32
                """
                # Optimized screen clearing for running state
                self.create_background_surface()  # Ensure background exists
                if self.use_dirty_rects:
                    self.clear_dirty_areas()  # Clear only changed areas
                else:
                    self.screen.fill(WHITE)   # Fallback to full screen clear
                
                # Draw background elements
                self.cloud.draw_clouds(self.screen)
                
                # Track major game areas as dirty rectangles
                self.add_dirty_rect(pygame.Rect(0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100))  # Ground area
                self.add_dirty_rect(pygame.Rect(0, 0, SCREEN_WIDTH, 50))                     # Score area
                self.add_dirty_rect(pygame.Rect(self.dino.dino_x - 10, self.dino.dino_y - 10, 
                                               80, 80))                                       # Dino area
                
                # Check for score progression (when dino passes obstacles)
                if self.obstacles.obstacle_removed == True:
                    self.score += 1
                    
                    # Increase difficulty every 5 points
                    if self.score % 5 == 0:
                        self.ground.ground_moving_speed += 1    # Faster ground
                        self.obstacles.obstacle_speed += 1      # Faster obstacles
                        
                    self.obstacles.obstacle_removed = False
                
                # Display current score
                self.display_score(self.screen)
                
                # Update and draw moving ground
                self.ground.move_ground(self.screen)

                # Handle dino state and animation
                if self.dino.dino_state == DINO_RUNNING:
                    self.dino.run_Dino_run(self.screen)
                    
                if self.dino.dino_state == DINO_JUMP:
                    self.dino.jump_Dino_jump(self.screen)
                    # Calculate and send jump height percentage to STM32
                    self.dino.Dino_jump_height_perc()
                    self.COM_port.send_to_com_port(self.dino.dino_jump_height_perc)

                # Update and draw obstacles
                self.obstacles.draw_obstacles(self.screen)
                
                # Track obstacle areas as dirty rectangles
                for obstacle in self.obstacles.obstacles:
                    if obstacle[0] < SCREEN_WIDTH:  # Only track visible obstacles
                        obstacle_rect = pygame.Rect(obstacle[0] - 10, obstacle[1] - 10, 
                                                   obstacle[2].get_width() + 20, 
                                                   obstacle[2].get_height() + 20)
                        self.add_dirty_rect(obstacle_rect)

                # Check for collisions between dino and obstacles
                self.dino.check_Dino_stuck(self.obstacles)
                if self.dino.dino_state == DINO_STUCK:
                    # Collision detected - transition to game over
                    self.game_state = GAME_OVER
                    self.display_game_over(self.screen)

            # Optimized display updates with dirty rectangles
            if self.use_dirty_rects and len(self.dirty_rects) > 0:
                # Update only the dirty (changed) areas instead of full screen
                pygame.display.update(self.dirty_rects)
                self.dirty_rects.clear()  # Clear dirty rects for next frame
            else:
                # Fallback to full screen update if dirty rects disabled or empty
                pygame.display.flip()

# ======================= PROGRAM ENTRY POINT =======================
if __name__ == "__main__":
    """
    Application entry point
    
    Creates and starts the main game instance when script is run directly.
    """
    game = DinoGame()  # Create game instance
    game.run()         # Start the main game loop
