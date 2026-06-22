#ifndef _ALGORITHM_
#define _ALGORITHM_
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

const int BOARD_SIZE = 15;      
inline int MAX_DEPTH = 0; 
inline int MAX_MOVES_TO_CHECK = 0; 
inline int optAlgorithm = 0;         
inline bool optMoveOrdering = false; 
inline long long stateCount =0;
inline int aiMoveScore =0;
inline int turnCounter =0;

struct Move {
    int row, col;
};

struct MoveScore {
    int row, col;
    int score;
    bool operator<(const MoveScore& other) const {
        return score > other.score;     
    }
};

void logGameTurn(int turn, Move humanMove, Move aiMove, double time, int aiScore, long long states);
void logNewGameHeader(); 
int getLineScore(int count, int block);
int evaluateBoard(int board[15][15]);
int evaluateCell(int board[15][15], int r, int c);
std::vector<Move> getTopMoves(int board[15][15]) ;
int checkWin(int board[15][15]);
int minimax(int board[15][15], int depth, int alpha, int beta, bool player,int opt,bool moveOrder);
Move findBestMove(int board[15][15],bool moveOrder);
#endif