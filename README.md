# 🎮 CARO GAME 
---
🔎 Overview
- **A simple 15x15 C++ Caro game (Gomoku variant) played between a Human (Player) and the Computer (AI). The system integrates an intuitive graphical user interface (GUI) alongside highly configurable adversarial search algorithms. The objective of the game is to form an unbroken chain of 5 moves (vertically, horizontally, diagonally) and the first player to do that wins the game.**

---
## 👥 Author
- **Nguyen Kieu Phong** 
---
🚀 INSTALLATION & RUN GUIDE
- **In order to run this program from an IDE on Windows OS you'll need a C++ compiler supporting at least the C++11 standard (GCC/MinGW, MSVC)**
- Assuming that you use g++ to compile .cpp file , then you need to go into the folder where you save the main.cpp file and use command:  g++ main.cpp graphics.cpp algorithm.cpp resource.o -o XO_AI.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows 
- **There will be a XO_AI.exe file in the folder after compilation. Continue to command : XO_AI in Command Prompt or ./XO_AI if you use VsCode. The game console will appear**
---
## ✨ KEY FEATURES

### 1. Smooth Graphical User Interface (GUI)
- Built on top of the lightweight **Raylib** library (C++), ensuring a responsive 60 FPS experience.
- Visual move highlighting (Red border for Player X, Blue border for AI O) to track the latest moves easily.
- Translucent status overlay upon game completion (`VICTORY!`, `GAME OVER`, `DRAW GAME`).
  
### 2. Live Algorithm Setup Popup
Allows players to dynamically tweak and configure the AI's "brain" mid-game via an interactive setup overlay:
- **Algorithm Selection:** Toggle seamlessly between pure **Minimax** and optimized **Alpha-Beta Pruning**.
- **Move Ordering:** Enable/Disable the heuristic pre-sorting of promising moves to maximize pruning efficiency.
- **AI Depth:** Input the search depth limit directly from the keyboard.
- **Top Moves:** Restrict the branching factor by expanding only the most optimal candidate moves, preventing state space explosion on the 15x15 grid.

### 3. Performance Logging System
- Automatically dumps real-time search metrics into `ai_log.txt` right after every AI move.
- Logs include: *Turn counter, placement coordinates, execution time (ms), evaluation score (Score), and total evaluated states (`stateCount`)*.

---

## 🛠️ PROJECT DIRECTORY STRUCTURE

```text
├── include/
│   └── graphics.h      # GUI configurations, button layouts, and rendering declarations
├── src/
│   ├── main.cpp        # Main entry point handling the core game loop
│   ├── graphics.cpp    # Definitions for InitGUI, HandleGUIInput, and DrawGUI
│   └── ai.cpp          # Implementation of Minimax, Alpha-Beta, and Heuristics
├── resources/
│   ├── icon.png        # Application window icon
│   └── VNARIAL-BOLD.TTF# Custom TrueType font for UI rendering
├── ai_log.txt          # Auto-generated runtime logs capturing AI search statistics
└── README.md           # Project documentation and guide
