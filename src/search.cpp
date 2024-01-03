#include "search.h"
#include <iostream>
#include <thread>

std::pair<bool, LegalMove> SearchManager::calculateBestMove(Board board, int depth, bool debug)
{
	LegalMove bestMove{};
	bool moveFound = false;

	if (depth <= 0)
		return std::make_pair(moveFound, bestMove);

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
				moveFound = true;
			}
		}
	}

	if (debug)
		performanceArgs.printPerformance();

	return std::make_pair(moveFound, bestMove);
}

int SearchManager::minimax(Board board, PieceColor playerColor, SearchArgs searchArgs, PerformanceArgs* performanceArgs, bool debug)
{
	searchArgs.curDepth++;

	if (searchArgs.curDepth >= searchArgs.maxDepth && (board.isQuiet() || searchArgs.curDepth < searchArgs.maxDepth + qLimit)) {
		int value = board.evalBoard(playerColor);

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
		if (newBoard.makeMove(move)) // performed move was not pseudolegal
		{
			int value = minimax(newBoard, playerColor, searchArgs, performanceArgs, debug);

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
