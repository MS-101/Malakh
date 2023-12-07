#ifndef AI_H
#define AI_H

#include "Board/board.h"
#include <chrono>

struct PerformanceArgs {
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	int positionsCur = 0;
	int positionsTotal = 0;
	long long durationTotal = 0;

	void printPerformance();
	void printPerformance(std::chrono::high_resolution_clock::time_point stop, long long durationCur);
};

struct SearchArgs {
	int curDepth = 0;
	int maxDepth = 0;
	int alpha = INT_MIN;
	int beta = INT_MAX;
};

class AI {
public:
	LegalMove calculateBestMove(Board* board, int depth, bool debug);
	LegalMove calculateBestMove_threads(Board* board, int depth, int workerCount, bool debug);
	int evaluate(Board* board, PieceColor playerColor);
private:
	int minimax(Board* board, LegalMove move, PieceColor playerColor, SearchArgs searchArgs, PerformanceArgs* performanceArgs, bool debug);
	int positionsPerDebugMessage = 1000;
};

#endif
