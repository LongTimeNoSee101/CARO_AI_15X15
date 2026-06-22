# 🎮 CARO GAME 
---
🔎 Overview
---
A simple 15x15 Caro game (Gomoku variant) played between a Human (Player) and the Computer (AI). The system integrates an intuitive graphical user interface (GUI) alongside highly configurable adversarial search algorithms.

---

## 👥 Au
- **J** - Student ID: `2021XXXX` - *(Role: AI Algorithms & Heuristic Evaluation Function)*

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
