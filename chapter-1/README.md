# Enhanced SFML Pong 🕹

A modern take on the classic arcade game Pong, built with C++ and the Simple and Fast Multimedia Library (SFML). This project features two-player local gameplay, a clean UI, a scoring system, and a clear win condition.



## Features

* **Classic Two-Player Gameplay:** Go head-to-head with a friend in local multiplayer.
* **Smooth, Responsive Controls:** Frame-rate independent physics for consistent gameplay.
* **Clear UI:** An instruction screen, in-game score display, and a "Game Over" screen make the experience seamless.
* **Winning Condition:** The first player to score **5 points** wins the match!
* **Simple Game Flow:** Start, play, win, and restart with simple key presses.

---

##  How to Play

The objective is to score points by getting the ball past your opponent's paddle.

* **Player 1 (Left Paddle):**
    * `W`: Move Up
    * `S`: Move Down
* **Player 2 (Right Paddle):**
    * `Up Arrow`: Move Up
    * `Down Arrow`: Move Down
* **Game Controls:**
    * `Spacebar`: Start the game from the instruction screen.
    * `R`: Restart the game at any time.
    * `Esc`: Quit the game.

---

##  Getting Started

To compile and run this game on your own machine, follow these steps.

### Prerequisites

1.  **A C++ Compiler:** A modern compiler that supports C++11 or newer (e.g., GCC, Clang, MSVC).
2.  **SFML Library:** You need to have SFML (version 2.5 or newer) installed. You can find installation instructions on the [official SFML website](https://www.sfml-dev.org/tutorials/2.6/).
3.  **Font File:** The game requires a font file.
    * Create a folder named `resources` in the same directory as your source code.
    * Place a TrueType Font file named `font.ttf` inside the `resources` folder. You can use any `.ttf` font you like, just rename it accordingly.

### Directory Structure

Your project folder should look like this before compiling:
`pong-project/
├── main.cpp
└── resources/
└── font.ttf`
---

##  Compilation and Execution

### On Linux or macOS

1.  **Open a terminal** and navigate to your project directory.
2.  **Compile the code** using the following command. This links the necessary SFML libraries.
    ```bash
    g++ main.cpp -o pong -lsfml-graphics -lsfml-window -lsfml-system
    ```
3.  **Run the game:**
    ```bash
    ./pong
    ```

### On Windows (with MinGW/GCC)

The compilation command is the same as for Linux. Ensure that your SFML library files are correctly located where the compiler can find them.

### On Windows (with Visual Studio)

You will need to configure your project's properties to link against SFML correctly:
1.  Add the SFML `include` directory to your **C/C++ -> General -> Additional Include Directories**.
2.  Add the SFML `lib` directory to your **Linker -> General -> Additional Library Directories**.
3.  Add the necessary `.lib` files (e.g., `sfml-graphics-d.lib`, `sfml-window-d.lib`, `sfml-system-d.lib` for debug mode) to **Linker -> Input -> Additional Dependencies**.
4.  Copy the SFML `.dll` files to the same directory as your final executable.

---

##  Dependencies

* **[SFML (Simple and Fast Multimedia Library)](https://www.sfml-dev.org/)**: The core library used for graphics, windowing, and input.
