
#include "include/graphics.h"

int main() {
    const int windowWidth = BOARD_SIZE * CELL_SIZE + SIDEBAR_WIDTH; 
    const int windowHeight = BOARD_SIZE * CELL_SIZE;                

    MAX_DEPTH = 1; 
    MAX_MOVES_TO_CHECK = 0; 

    InitGUI(windowWidth, windowHeight);

    
    
    int board[15][15] = {0};

    int gameState = 0; 

    while (!WindowShouldClose()) {
        HandleGUIInput(board, gameState);
        
        DrawGUI(board, gameState, windowWidth, windowHeight);
    }

    UnloadGUI();
    return 0;
}