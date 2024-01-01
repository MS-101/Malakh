#include "board.h"
#include <chrono>

struct SearchArgs {
	int curDepth = 0;
	int maxDepth = 0;
	int alpha = INT_MIN;
	int beta = INT_MAX;
};

struct PerformanceArgs {
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	int positionsCur = 0;
	int positionsTotal = 0;
	long long durationTotal = 0;

	void printPerformance();
	void printPerformance(std::chrono::high_resolution_clock::time_point stop, long long durationCur);
};

class SearchManager {
public:
	static LegalMove calculateBestMove(Board board, int depth, bool debug);
	static int minimax(Board board, PieceColor playerColor, SearchArgs searchArgs, PerformanceArgs* performanceArgs, bool debug);
};
