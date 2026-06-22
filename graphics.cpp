#include "include/graphics.h"
#include <chrono>
#include <fstream>

void InitGUI(int windowWidth, int windowHeight) {
    InitWindow(windowWidth, windowHeight, "CARO 15x15 ");
    SetTargetFPS(60);

    Image gameIcon = LoadImage("icon.png"); 
    ImageFormat(&gameIcon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(gameIcon);
    UnloadImage(gameIcon);
    
    myFont = LoadFontEx("VNARIAL-BOLD.TTF", 48, 0, 0);
}

void HandleGUIInput(int board[15][15], int& gameState) {
    mousePos = GetMousePosition();
    int mouseX = (int)mousePos.x;
    int mouseY = (int)mousePos.y;

    btnSetupHovered = CheckCollisionPointRec(mousePos, btnSetup);
    btnResetHovered = CheckCollisionPointRec(mousePos, btnReset);

    if (showSetup && activeInput > 0) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9') {
                if (activeInput == 1) {
                    MAX_DEPTH = (key - '0'); 
                } else if (activeInput == 2) {
                    MAX_MOVES_TO_CHECK = (key - '0'); 
                }
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (activeInput == 1) MAX_DEPTH = 0; 
            if (activeInput == 2) MAX_MOVES_TO_CHECK = 0; 
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        
        if (showSetup) {
            int windowWidth = BOARD_SIZE * CELL_SIZE + SIDEBAR_WIDTH;
            int windowHeight = BOARD_SIZE * CELL_SIZE;
            Rectangle popupRec = { (float)(windowWidth - 460) / 2.0f, (float)(windowHeight - 380) / 2.0f, 460.0f, 380.0f };
            Rectangle btnClose = { popupRec.x + popupRec.width - 140, popupRec.y + popupRec.height - 55, 110, 35 };

            if (CheckCollisionPointRec(mousePos, btnClose)) {
                showSetup = false; 
                activeInput = 0; 
            }

            Rectangle setupArea = { popupRec.x + 25, popupRec.y + 60, 410, 245 };
            if (CheckCollisionPointRec(mousePos, setupArea)) {
                float itemY = popupRec.y + 90.0f;
                float itemSpacing = 56.0f;

                Rectangle recMinimax = { setupArea.x + 160, itemY - 5, 100, 30 };
                Rectangle recAlphaBeta = { setupArea.x + 270, itemY - 5, 110, 30 };
                if (CheckCollisionPointRec(mousePos, recMinimax)) { optAlgorithm = 0; activeInput = 0; }
                if (CheckCollisionPointRec(mousePos, recAlphaBeta)) { optAlgorithm = 1; activeInput = 0; }

                itemY += itemSpacing;
                Rectangle recMoveOrdering = { setupArea.x + 160, itemY - 5, 100, 30 };
                if (CheckCollisionPointRec(mousePos, recMoveOrdering)) { optMoveOrdering = !optMoveOrdering; activeInput = 0; }

                itemY += itemSpacing;
                Rectangle recDepthBox = { setupArea.x + 160, itemY - 5, 100, 30 };
                if (CheckCollisionPointRec(mousePos, recDepthBox)) activeInput = 1;

                itemY += itemSpacing;
                Rectangle recTopMovesBox = { setupArea.x + 160, itemY - 5, 100, 30 };
                if (CheckCollisionPointRec(mousePos, recTopMovesBox)) activeInput = 2;

                if (!CheckCollisionPointRec(mousePos, recDepthBox) && !CheckCollisionPointRec(mousePos, recTopMovesBox) &&
                    !CheckCollisionPointRec(mousePos, recMinimax) && !CheckCollisionPointRec(mousePos, recAlphaBeta) && 
                    !CheckCollisionPointRec(mousePos, recMoveOrdering)) {
                    activeInput = 0;
                }
            } else {
                if (!CheckCollisionPointRec(mousePos, btnClose)) activeInput = 0;
            }
        } 
        else {
            if (gameState == 0 && mouseX < BOARD_SIZE * CELL_SIZE) {
                int col = mouseX / CELL_SIZE;
                int row = mouseY / CELL_SIZE;

                if (board[row][col] == 0) {
                    board[row][col] = 1; 
                    currentX.first = row;
                    currentX.second = col;
                    gameState = checkWin(board);
                    if (gameState == 1) {
                        logGameTurn(++turnCounter, {row,col}, {-1,-1}, 0, 0, 0);
                    }
                    if (gameState == 0) {
                        turnCounter++;
                        auto start = std::chrono::high_resolution_clock::now();
                        Move aiMove = findBestMove(board, optMoveOrdering); 
                        auto end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> elapsed = end - start;
                        double timeTakenMs = elapsed.count();
                        
                        if (turnCounter == 1) {
                           std::ofstream logFile("ai_log.txt", std::ios::trunc); 
                        }
                        
                        logGameTurn(turnCounter, {row,col}, aiMove, timeTakenMs, aiMoveScore, stateCount);
                        stateCount = 0;
                        if (aiMove.row != -1 && aiMove.col != -1) {
                            board[aiMove.row][aiMove.col] = 2; 
                            currentO.first = aiMove.row;
                            currentO.second = aiMove.col;
                        }
                        gameState = checkWin(board); 
                    }
                }
            }
        }

        if (btnSetupHovered) {
            showSetup = true;
            activeInput = 0;
        }
        if (btnResetHovered && !showSetup) {
            gameState = 0;
            for (int r = 0; r < BOARD_SIZE; r++) {
                for (int c = 0; c < BOARD_SIZE; c++) board[r][c] = 0; 
            }
            currentX = currentO = std::pair<int,int>(-1,-1);
            turnCounter = 0;
            stateCount = 0;
        }
    }
}

void DrawGUI(int board[15][15], int gameState, int windowWidth, int windowHeight) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i <= BOARD_SIZE; i++) {
        DrawLine(0, i * CELL_SIZE, BOARD_SIZE * CELL_SIZE, i * CELL_SIZE, BLACK);
        DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, BOARD_SIZE * CELL_SIZE, BLACK);
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int centerX = c * CELL_SIZE + CELL_SIZE / 2;
            int centerY = r * CELL_SIZE + CELL_SIZE / 2;

            if (board[r][c] == 1) { 
                int offset = 10;
                DrawLineEx({ (float)(c * CELL_SIZE + offset), (float)(r * CELL_SIZE + offset) },
                           { (float)((c + 1) * CELL_SIZE - offset), (float)((r + 1) * CELL_SIZE - offset) }, 3, RED);
                DrawLineEx({ (float)((c + 1) * CELL_SIZE - offset), (float)(r * CELL_SIZE + offset) },
                           { (float)(c * CELL_SIZE + offset), (float)((r + 1) * CELL_SIZE - offset) }, 3, RED);
            } 
            else if (board[r][c] == 2) { 
                DrawRing({ (float)centerX, (float)centerY }, 8, 12, 0, 360, 0, BLUE);
            }

            if (currentX != std::pair<int,int>(-1,-1)) {
                Rectangle rec = { (float)currentX.second*CELL_SIZE, (float)currentX.first*CELL_SIZE, (float)CELL_SIZE, (float)CELL_SIZE };
                DrawRectangleLinesEx(rec, 2.5f, RED); 
            }

            if (currentO != std::pair<int,int>(-1,-1)) {
                Rectangle rec = { (float)currentO.second*CELL_SIZE, (float)currentO.first*CELL_SIZE, (float)CELL_SIZE, (float)CELL_SIZE };
                DrawRectangleLinesEx(rec, 2.5f, BLUE); 
            }
        }
    }

    DrawLine(BOARD_SIZE * CELL_SIZE, 0, BOARD_SIZE * CELL_SIZE, windowHeight, DARKGRAY);
    DrawText("CARO 15x15", 645, 30, 20, MAROON);
    DrawTextEx(myFont, "MATCH STATUS", {615, 80}, 20, 1.0f, DARKGRAY);

    if (gameState == 0) {
        Vector2 Size = MeasureTextEx(myFont, "IN GAME", 18, 1.0f);
        float x = BOARD_SIZE * CELL_SIZE + ((SIDEBAR_WIDTH - Size.x) / 2.0f);
        DrawTextEx(myFont, "IN GAME", {x, 110}, 18, 1.0f, GREEN);
    }

    DrawRectangleRec(btnSetup, (btnSetupHovered && !showSetup)? GRAY : DARKGRAY);
    DrawRectangleLinesEx(btnSetup, 2, BLACK);
    DrawTextEx(myFont, "SETUP", {btnSetup.x + 43, btnSetup.y + 15}, 20, 1.0f, WHITE);

    DrawRectangleRec(btnReset, (btnResetHovered && !showSetup)? GRAY : DARKGRAY);
    DrawRectangleLinesEx(btnReset, 2, BLACK);
    DrawTextEx(myFont, "RESET", {btnReset.x + 43, btnReset.y + 15}, 20, 1.0f, WHITE);

    if (gameState != 0) {
        DrawRectangle(0, 0, BOARD_SIZE * CELL_SIZE, windowHeight, Fade(BLACK, 0.4f));
        const char* endText1 = "";
        const char* endText2 = "";
        Color textColor = WHITE;

        if (gameState == 1) { endText1 = "VICTORY!"; endText2 = "PLAYER WIN!"; textColor = GREEN; } 
        else if (gameState == 2) { endText1 = "GAME OVER"; endText2 = "AI WIN!"; textColor = RED; } 
        else if (gameState == 3) { endText1 = "DRAW GAME"; endText2 = "DRAW GAME!"; textColor = GOLD; } 
    
        float fontSize1 = 50.0f; float fontSize2 = 18.0f;
        float spacing1 = 2.0f; float spacing2 = 1.0f;
    
        Vector2 textSize1 = MeasureTextEx(myFont, endText1, fontSize1, spacing1);
        Vector2 textSize2 = MeasureTextEx(myFont, endText2, fontSize2, spacing2);
   
        float boardWidth = BOARD_SIZE * CELL_SIZE;  
        float posX1 = (boardWidth - textSize1.x) / 2.0f;
        float posY1 = (windowHeight - textSize1.y) / 2.0f;
   
        float posX2 = BOARD_SIZE * CELL_SIZE + ((SIDEBAR_WIDTH - textSize2.x) / 2.0f);
        float posY2 = 110;

        DrawTextEx(myFont, endText1, { posX1, posY1 }, fontSize1, spacing1, textColor);
        DrawTextEx(myFont, endText2, { posX2, posY2 }, fontSize2, spacing2, textColor);
    }

    if (showSetup) {
        DrawRectangle(0, 0, windowWidth, windowHeight, Fade(BLACK, 0.6f));

        Rectangle popupRec = { (float)(windowWidth - 460) / 2.0f, (float)(windowHeight - 380) / 2.0f, 460.0f, 380.0f };
        DrawRectangleRec(popupRec, RAYWHITE);
        DrawRectangleLinesEx(popupRec, 3, DARKGRAY);

        DrawTextEx(myFont, "ALGORITHM SETUP", { popupRec.x + 25, popupRec.y + 20 }, 24, 1.0f,MAROON);
        DrawLine(popupRec.x + 25, popupRec.y + 55, popupRec.x + popupRec.width - 25, popupRec.y + 55, LIGHTGRAY);

        float itemX = popupRec.x + 25;
        float itemY = popupRec.y + 90.0f;
        float itemSpacing = 56.0f; 

        DrawTextEx(myFont, "1. Algorithm", { itemX, itemY }, 18, 1.0f, DARKGRAY);
        
        Rectangle btnMinimax = { itemX + 160, itemY - 5, 100, 30 };
        Rectangle btnAlphaBeta = { itemX + 270, itemY - 5, 110, 30 };
        
        DrawRectangleRec(btnMinimax, (optAlgorithm == 0) ? LIME : LIGHTGRAY);
        DrawRectangleLinesEx(btnMinimax, 1, BLACK);
        
        Vector2 sizeMinimax = MeasureTextEx(myFont, "Minimax", 16, 1.0f);
        float mx = btnMinimax.x + (btnMinimax.width - sizeMinimax.x) / 2.0f;
        float my = btnMinimax.y + (btnMinimax.height - sizeMinimax.y) / 2.0f;
        DrawTextEx(myFont, "Minimax", { mx, my }, 16, 1.0f, (optAlgorithm == 0) ? BLACK : DARKGRAY);

        DrawRectangleRec(btnAlphaBeta, (optAlgorithm == 1) ? LIME : LIGHTGRAY);
        DrawRectangleLinesEx(btnAlphaBeta, 1, BLACK);
        
        Vector2 sizeAlphaBeta = MeasureTextEx(myFont, "Alpha-Beta", 16, 1.0f);
        float ax = btnAlphaBeta.x + (btnAlphaBeta.width - sizeAlphaBeta.x) / 2.0f;
        float ay = btnAlphaBeta.y + (btnAlphaBeta.height - sizeAlphaBeta.y) / 2.0f;
        DrawTextEx(myFont, "Alpha-Beta", { ax, ay }, 16, 1.0f, (optAlgorithm == 1) ? BLACK : DARKGRAY);

        itemY += itemSpacing;
        DrawTextEx(myFont, "2. Move Order", { itemX, itemY }, 18, 1.0f, DARKGRAY);
        
        Rectangle btnMoveOrdering = { itemX + 160, itemY - 5, 100, 30 };
        DrawRectangleRec(btnMoveOrdering, optMoveOrdering ? LIME : LIGHTGRAY);
        DrawRectangleLinesEx(btnMoveOrdering, 1, BLACK);
        
        const char* strMoveOrdering = optMoveOrdering ? "ON" : "OFF";
        Vector2 sizeMoveOrdering = MeasureTextEx(myFont, strMoveOrdering, 16, 1.0f);
        float mox = btnMoveOrdering.x + (btnMoveOrdering.width - sizeMoveOrdering.x) / 2.0f;
        float moy = btnMoveOrdering.y + (btnMoveOrdering.height - sizeMoveOrdering.y) / 2.0f;
        DrawTextEx(myFont, strMoveOrdering, { mox, moy }, 16, 1.0f, BLACK);

        itemY += itemSpacing;
        DrawTextEx(myFont, "3. AI Depth", { itemX, itemY }, 18, 1.0f, DARKGRAY);
        
        Rectangle txtDepthBox = { itemX + 160, itemY - 5, 100, 30 };
        DrawRectangleRec(txtDepthBox, LIGHTGRAY);
        DrawRectangleLinesEx(txtDepthBox, (activeInput == 1) ? 2.5f : 1.0f, (activeInput == 1) ? LIGHTGRAY : BLACK);
        
        const char* strDepth = TextFormat("%d", MAX_DEPTH);
        Vector2 sizeDepth = MeasureTextEx(myFont, strDepth, 16, 1.0f);
        float dx = txtDepthBox.x + (txtDepthBox.width - sizeDepth.x) / 2.0f;
        float dy = txtDepthBox.y + (txtDepthBox.height - sizeDepth.y) / 2.0f;
        DrawTextEx(myFont, strDepth, { dx, dy }, 16, 1.0f, BLACK);
        
        if (activeInput == 1 && (GetTime() - (int)GetTime() > 0.5)) { 
            DrawLine((int)(dx + sizeDepth.x + 2), (int)txtDepthBox.y + 6, (int)(dx + sizeDepth.x + 2), (int)txtDepthBox.y + 24, BLACK);
        }

        itemY += itemSpacing;
        DrawTextEx(myFont, "4. Top Moves", { itemX, itemY }, 18, 1.0f, DARKGRAY);
        
        Rectangle txtTopMovesBox = { itemX + 160, itemY - 5, 100, 30 };
        DrawRectangleRec(txtTopMovesBox, LIGHTGRAY);
        DrawRectangleLinesEx(txtTopMovesBox, (activeInput == 2) ? 2.5f : 1.0f, (activeInput == 2) ? LIGHTGRAY: BLACK);
        
        const char* strTopMoves = TextFormat("%d", MAX_MOVES_TO_CHECK);
        Vector2 sizeTopMoves = MeasureTextEx(myFont, strTopMoves, 16, 1.0f);
        float tmx = txtTopMovesBox.x + (txtTopMovesBox.width - sizeTopMoves.x) / 2.0f;
        float tmy = txtTopMovesBox.y + (txtTopMovesBox.height - sizeTopMoves.y) / 2.0f;
        DrawTextEx(myFont, strTopMoves, { tmx, tmy }, 16, 1.0f, BLACK);
        
        if (activeInput == 2 && (GetTime() - (int)GetTime() > 0.5)) {
            DrawLine((int)(tmx + sizeTopMoves.x + 2), (int)txtTopMovesBox.y + 6, (int)(tmx + sizeTopMoves.x + 2), (int)txtTopMovesBox.y + 24, BLACK);
        }

        Rectangle btnClose = { popupRec.x + popupRec.width - 140, popupRec.y + popupRec.height - 55, 110, 35 };
        bool closeHover = CheckCollisionPointRec(mousePos, btnClose);
        
        DrawRectangleRec(btnClose, closeHover ? MAROON:DARKGRAY);
        DrawRectangleLinesEx(btnClose, 1, BLACK);
        
        Vector2 closeSize = MeasureTextEx(myFont, "CLOSE", 16, 1.0f);
        float closeX = btnClose.x + (btnClose.width - closeSize.x) / 2.0f;
        float closeY = btnClose.y + (btnClose.height - closeSize.y) / 2.0f;
        DrawTextEx(myFont, "CLOSE", { closeX, closeY }, 16, 1.0f, WHITE);
    }

    EndDrawing();
}

void UnloadGUI() {
    UnloadFont(myFont);
    CloseWindow();
}