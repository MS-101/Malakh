#ifndef AI_H
#define AI_H

#include "Board/board.h"
#include <chrono>

class AI {
public:
	LegalMove* calculateBestMove(Board* board, int depth);
	int evaluate(Board* board, PieceColor playerColor);
private:
	struct minimaxResponse minimax(Board* board, LegalMove* move, PieceColor playerColor, int depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point start, int positionsTotal, long long durationTotal);
};

struct minimaxResponse {
	int value;
	std::chrono::high_resolution_clock::time_point start;
	int positionsTotal;
	long long durationTotal;
};

#endif
