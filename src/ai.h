#ifndef AI_H
#define AI_H

#include "Board/board.h"
#include <chrono>

struct minimaxResponse {
	int value;
	std::chrono::high_resolution_clock::time_point start;
	int positionsTotal;
	long long durationTotal;
};

class AI {
public:
	legalMove calculateBestMove(Board* board, int depth);
	legalMove calculateBestMove_threads(Board* board, int depth);
	int evaluate(Board* board, PieceColor playerColor);
private:
	minimaxResponse minimax(Board* board, legalMove move, PieceColor playerColor, int depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point start, int positionsTotal, long long durationTotal);
	int positionsPerDebugMessage = 1000;
};

#endif
