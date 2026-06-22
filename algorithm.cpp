#include "include/algorithm.h"

void logGameTurn(int turn, Move humanMove, Move aiMove, double time, int aiScore, long long states) {
    std::ofstream logFile("ai_log.txt", std::ios::app);     
    if (logFile.is_open()) {
        logFile << "Turn: " << turn << "\n"
                << "Human Move: (" << humanMove.row << "," << humanMove.col << ") \n "
                << "AI Move: (" << aiMove.row << "," << aiMove.col << ") \n "
                << "AI Score: " << aiScore << " \n "
                << "States Evaluated: " << states << " \n "
                << "Response Time: " << std::fixed << std::setprecision(2) << time << " ms\n\n";
        logFile.close();
    }
}




int getLineScore(int count, int block) {
    if (block == 2 && count < 5) return 0; 
    if (count >= 5) return 100000;
    if (count == 4) return (block == 0) ? 10000 : 1000;
    if (count == 3) return (block == 0) ? 1000 : 100;
    if (count == 2) return (block == 0) ? 100 : 10;
    return 1;
}

int evaluateBoard(int board[15][15]) {
    int score = 0;
    int row_cord[] = {0, 1, 1, 1};
    int col_cord[] = {1, 0, 1, -1};

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c] == 0) continue;
            int value = board[r][c];

            for (int i = 0; i < 4; i++) {
                int count = 1;
                int block = 0;
                
                while (count < 5) {
                    int new_r = r + row_cord[i] * count;
                    int new_c = c + col_cord[i] * count;
                    if (new_r < 0 || new_r >= BOARD_SIZE || new_c < 0 || new_c >= BOARD_SIZE) { block++; break; }
                    if (board[new_r][new_c] == value) count++;
                    else if (board[new_r][new_c] != 0) { block++; break; }
                    else break;
                }

          
                int back_r = r - row_cord[i];
                int back_c = c - col_cord[i];
                if (back_r < 0 || back_r >= BOARD_SIZE || back_c < 0 || back_c >= BOARD_SIZE || (board[back_r][back_c] != 0 && board[back_r][back_c] != value)) {
                    block++;
                }

                int weight = getLineScore(count, block);
                if (value == 2) score += weight;
                else score -= weight;
            }
        }
    }
    return score;
}


int evaluateCell(int board[15][15], int r, int c) {
    int score = 0;
    board[r][c] = 2;
    score += abs(evaluateBoard(board)); 
    board[r][c] = 1;
    score += abs(evaluateBoard(board)); 
    board[r][c] = 0; 
    return score;
}


std::vector<Move> getTopMoves(int board[15][15]) {
    std::vector<MoveScore> goodMoves;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c] == 0) {
                bool hasNeighbor = false;
                for (int i = -2; i <= 2 && !hasNeighbor; i++) {
                    for (int j = -2; j <= 2 && !hasNeighbor; j++) {
                        int new_r = r + i, new_c = c + j;
                        if (new_r >= 0 && new_r < BOARD_SIZE && new_c >= 0 && new_c < BOARD_SIZE && board[new_r][new_c] != 0) {
                            hasNeighbor = true;
                            break;
                        }
                    }
                    if(hasNeighbor) break;
                }
                
                if (!hasNeighbor && r == BOARD_SIZE/2 && c == BOARD_SIZE/2) hasNeighbor = true;

                if (hasNeighbor) {
                    int score = evaluateCell(board, r, c);
                    goodMoves.push_back({r, c, score});
                }
            }
        }
    }

    std::sort(goodMoves.begin(), goodMoves.end());

    std::vector<Move> topMoves;
    int limit = std::min((int)goodMoves.size(), MAX_MOVES_TO_CHECK);
    for (int i = 0; i < limit; i++) {
        topMoves.push_back({goodMoves[i].row, goodMoves[i].col});
    }
    return topMoves;
}


int checkWin(int board[15][15]) {
    int score = evaluateBoard(board);
    if (score >= 50000) return 2;  
    if (score <= -50000) return 1; 
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (board[r][c] == 0) return 0; 
    return 3; 
}


int minimax(int board[15][15], int depth, int alpha, int beta, bool player, int opt, bool moveOrder) {
    stateCount++;
    int status = checkWin(board);
    if (status == 2) return 100000 - depth;
    if (status == 1) return -100000 + depth;
    if (status == 3) return 0;
    if (depth == MAX_DEPTH) return evaluateBoard(board);


    if (moveOrder) {
        std::vector<Move> moves = getTopMoves(board);
        if (player) { 
            int maxEval = -1000000;
            for (auto move : moves) {
                board[move.row][move.col] = 2;
                int eval = minimax(board, depth + 1, alpha, beta, false, opt, moveOrder);
                board[move.row][move.col] = 0;
                maxEval = std::max(maxEval, eval);
                if (opt == 1) { 
                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) break; 
                }
            }
            return maxEval;
        } else { 
            int minEval = 1000000;
            for (auto move : moves) {
                board[move.row][move.col] = 1;
                int eval = minimax(board, depth + 1, alpha, beta, true, opt, moveOrder);
                board[move.row][move.col] = 0;
                minEval = std::min(minEval, eval);
                if (opt == 1) { 
                    beta = std::min(beta, eval);
                    if (beta <= alpha) break; 
                }
            }
            return minEval;
        }
    } 

    else { 
        if (player) { 
            int maxEval = -1000000;
            for (int r = 0; r < BOARD_SIZE; r++) {
                for (int c = 0; c < BOARD_SIZE; c++) {
                    if (board[r][c] == 0) {
                        board[r][c] = 2;
                        int eval = minimax(board, depth + 1, alpha, beta, false, opt, moveOrder);
                        board[r][c] = 0;
                        maxEval = std::max(maxEval, eval);
                        if (opt == 1) {
                            alpha = std::max(alpha, eval);
                            if (beta <= alpha) break;
                        }
                    }
                }
            }
            return maxEval;
        } else { 
            int minEval = 1000000;
            for (int r = 0; r < BOARD_SIZE; r++) {
                for (int c = 0; c < BOARD_SIZE; c++) {
                    if (board[r][c] == 0) {
                        board[r][c] = 1; 
                        int eval = minimax(board, depth + 1, alpha, beta, true, opt, moveOrder); 
                        board[r][c] = 0;
                        minEval = std::min(minEval, eval);
                        if (opt == 1) {
                            beta = std::min(beta, eval);
                            if (beta <= alpha)  break; 
                        }
                    }
                }
            }
        return minEval;
        } 
    }
}


Move findBestMove(int board[15][15], bool moveOrder) {
    int bestVal = -1000000;
    Move bestMove = {-1, -1};

    int localBoard[15][15];
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) localBoard[r][c] = board[r][c];
    }

    if (moveOrder) { 
        std::vector<Move> moves = getTopMoves(localBoard);
        for (auto move : moves) {
            localBoard[move.row][move.col] = 2;
            int moveValue = minimax(localBoard, 1, -1000000, 1000000, false, optAlgorithm, moveOrder);
            localBoard[move.row][move.col] = 0;

            if (moveValue > bestVal) {
                bestMove = move;
                bestVal = moveValue;
            }
        }
    } else { 
        for (int r = 0; r < 15; r++) {
            for (int c = 0; c < 15; c++) {
                if (localBoard[r][c] == 0) { 
                    localBoard[r][c] = 2;   
                    int moveVal = minimax(localBoard, 1, -1000000, 1000000, false, optAlgorithm, optMoveOrdering); 
                    localBoard[r][c] = 0;    

                    if (moveVal > bestVal) {
                        bestMove.row = r;
                        bestMove.col = c;
                        bestVal = moveVal;
                    }
                }
            }
        }
    }
    aiMoveScore = bestVal;
    return bestMove;
}