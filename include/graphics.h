#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "raylib.h"
#include "algorithm.h"
#include <utility>

inline const int CELL_SIZE = 40;       
inline const int SIDEBAR_WIDTH = 200;  

inline int activeInput = 0;          
inline bool showSetup = false;       
inline bool btnSetupHovered = false;
inline bool btnResetHovered = false;
inline std::pair<int,int> currentX{-1,-1};
inline std::pair<int,int> currentO{-1,-1};
inline Vector2 mousePos;

inline Font myFont;

inline Rectangle btnSetup = { 625, 230, 150, 50 }; 
inline Rectangle btnReset = { 625, 300, 150, 50 };

void InitGUI(int windowWidth, int windowHeight);
void HandleGUIInput(int board[15][15], int& gameState);
void DrawGUI(int board[15][15], int gameState, int windowWidth, int windowHeight);
void UnloadGUI();

#endif