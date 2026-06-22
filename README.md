# 🎮 CARO GAME 15x15

A simple 15x15 C++ Caro game (Gomoku variant) played between a Human (Player) and the Computer (AI). The system integrates an intuitive graphical user interface (GUI) alongside highly configurable adversarial search algorithms. The objective of the game is to form an unbroken chain of 5 moves (vertically, horizontally, diagonally) and the first player to do that wins the game.

---

## 👥 Author
- **Nguyen Kieu Phong**

---

## 🚀 Installation & Run Guide

### Option 1: Build from Source (For Developers)
In order to run this program from an IDE on Windows OS, you will need a C++ compiler supporting at least the **C++11** standard (such as GCC/MinGW or MSVC).

Assuming that you use `g++` to compile the `.cpp` files, navigate into the project root folder where your `main.cpp` is saved and execute the following command:

```bash
g++ main.cpp graphics.cpp algorithm.cpp resource.o -o XO_AI.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
```

After successful compilation, a file named `XO_AI.exe` will be generated in the directory. Run the game using one of the following commands:
* **Command Prompt:** `XO_AI`
* **VS Code Terminal:** `./XO_AI`

### Option 2: Quick Play (For Players)
If you just want to play the game immediately without installing any compiler or setting up development environments:
1. Go to the [Releases](https://github.com/LongTimeNoSee101/CARO_AI_15X15/releases/tag/V1.0.0) section on the right side of this repository.
2. Download the `XO_AI_v1.0.0.zip` package.
3. Extract the ZIP file and double-click `XO_AI.exe` to launch the game instantly!

---

## ✨ Key Features

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

## 🛠️ Project Directory Structure

Based on the development environment setup, the internal workspace architecture is organized as follows:

```text
├── include/
│   ├── algorithm.h
│   ├── graphics.h
│   └── raylib.h
├── lib/
│   └── libraylib.a
├── algorithm.cpp
├── graphics.cpp
├── icon.ico
├── icon.png
├── main.cpp
├── resource.o
├── resource.rc
├── VNARIAL-BOLD.TTF
```
