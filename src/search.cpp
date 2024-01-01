#include "search.h"
#include <iostream>
#include <thread>

LegalMove SearchManager::calculateBestMove(Board board, int depth, bool debug)
{
	LegalMove bestMove{};

	if (depth <= 0)
		return bestMove;

	PieceColor playerColor = board.curTurn;
	int max = INT_MIN;

	SearchArgs searchArgs{};
	searchArgs.maxDepth = depth;

	PerformanceArgs performanceArgs{};

	for (const LegalMove& move : board.moves[board.curTurn]) {
		Board newBoard = board;
		if (newBoard.makeMove(move)) {
			int value = minimax(newBoard, playerColor, searchArgs, &performanceArgs, debug);

			if (value > max) {
				max = value;
				bestMove = move;
			}
		}
	}

	if (debug)
		performanceArgs.printPerformance();

	return bestMove;
}

int SearchManager::minimax(Board board, PieceColor playerColor, SearchArgs searchArgs, PerformanceArgs* performanceArgs, bool debug)
{
	searchArgs.curDepth++;

	if (searchArgs.curDepth == searchArgs.maxDepth) {
		int value = 0;

		if (debug) {
			performanceArgs->positionsCur++;

			auto start = performanceArgs->start;
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

			long long durationCur = duration.count();
			if (durationCur > 1000)
				performanceArgs->printPerformance(stop, durationCur);
		}

		return value;
	}

	bool maximizingPlayer = playerColor == board.curTurn;
	int bestScore = maximizingPlayer ? INT_MIN : INT_MAX;

	for (const LegalMove& move : board.moves[board.curTurn]) {
		Board newBoard = board;
		newBoard.makeMove(move);

		int value = minimax(board, playerColor, searchArgs, performanceArgs, debug);

		if (maximizingPlayer) {
			bestScore = std::max(value, bestScore);
			searchArgs.alpha = std::max(searchArgs.alpha, bestScore);
		} else {
			bestScore = std::min(value, bestScore);
			searchArgs.beta = std::min(searchArgs.beta, bestScore);
		}

		if (searchArgs.beta <= searchArgs.alpha)
			break;
	}

	return bestScore;
}

void PerformanceArgs::printPerformance()
{
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	long long durationCur = duration.count();

	printPerformance(stop, durationCur);
}

void PerformanceArgs::printPerformance(std::chrono::high_resolution_clock::time_point stop, long long durationCur)
{
	positionsTotal += positionsCur;
	durationTotal += durationCur;

	std::cout << "Thread " << std::this_thread::get_id() << " explored "
		<< positionsCur << " positions (total " << positionsTotal << " positions) in "
		<< durationCur << " ms (total " << durationTotal << " ms)." << std::endl;

	positionsCur = 0;
	start = stop;
}
