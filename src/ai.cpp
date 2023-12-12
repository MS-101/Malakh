#include "ai.h"
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>


LegalMove AI::calculateBestMove(Board* board, int depth, bool debug)
{
	LegalMove bestMove{};

	if (depth <= 0)
		return bestMove;

	PieceColor playerColor = board->curTurn;
	int max = INT_MIN;

	SearchArgs searchArgs{};
	searchArgs.maxDepth = depth;

	PerformanceArgs* performanceArgs = new PerformanceArgs();

	std::vector<LegalMove> legalMoves = board->getLegalMoves(playerColor);
	for (const LegalMove& move : legalMoves) {
		int value = minimax(board, move, playerColor, searchArgs, performanceArgs, debug);
		if (value > max) {
			max = value;
			bestMove = move;
		}
	}

	if (debug)
		performanceArgs->printPerformance();

	delete performanceArgs;

	return bestMove;
}

LegalMove AI::calculateBestMove_threads(Board* board, int depth, int workerCount, bool debug)
{
	LegalMove bestMove{};

	if (depth <= 0)
		return bestMove;

	int max = INT_MIN;
	std::mutex maxMutex;

	PieceColor playerColor = board->curTurn;

	SearchArgs searchArgs{};
	searchArgs.maxDepth = depth;
	
	// create move queue
	std::vector<LegalMove> legalMoves = board->getLegalMoves(playerColor);
	std::mutex moveQueueMutex;
	std::queue<LegalMove> moveQueue;
	for (const LegalMove& move : board->getLegalMoves(playerColor))
		moveQueue.push(move);

	// create worker threads
	std::vector<std::pair<int, LegalMove>> bestMoves;
	bestMoves.assign(workerCount, std::make_pair(INT_MIN, bestMove));
	std::vector<std::thread> threads;
	for (int i = 0; i < workerCount; i++) {
		threads.emplace_back([&, i]() {
			Board* localBoard = new Board(board);
			PerformanceArgs* performanceArgs = new PerformanceArgs();

			while (true) {
				moveQueueMutex.lock();

				if (moveQueue.empty()) {
					moveQueueMutex.unlock();
					break;
				}

				LegalMove move = moveQueue.front();
				moveQueue.pop();

				moveQueueMutex.unlock();

				int value = minimax(localBoard, move, playerColor, searchArgs, performanceArgs, debug);

				std::pair<int, LegalMove> myBestMove = bestMoves[i];
				if (value > myBestMove.first)
					bestMoves[i] = std::make_pair(value, move);
			}

			if (debug)
				performanceArgs->printPerformance();

			delete performanceArgs;
			delete localBoard;
		});
	}

	// perform tasks
	for (std::thread& thread : threads)
		thread.join();

	for (std::pair<int, LegalMove>& curBestMove : bestMoves) {
		if (curBestMove.first > max) {
			max = curBestMove.first;
			bestMove = curBestMove.second;
		}
	}

	return bestMove;
}

int AI::minimax(Board* board, LegalMove move, PieceColor playerColor, SearchArgs searchArgs, PerformanceArgs* performanceArgs, bool debug)
{
	searchArgs.curDepth++;

	/*
	for (int i = 0; i < searchArgs.curDepth; i++)
		std::cout << "    ";
	std::cout << "board->makeMove(" << move.x1 << ", " << move.y1 << ", " << move.x2 << ", " << move.y2 << ", Pawn);" << std::endl;
	*/

	board->makeMove(move.x1, move.y1, move.x2, move.y2, move.promotionType);
	
	if (searchArgs.curDepth == searchArgs.maxDepth) {
		int value = evaluate(board, playerColor);

		if (debug) {
			performanceArgs->positionsCur++;

			auto start = performanceArgs->start;
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

			long long durationCur = duration.count();
			if (durationCur > 1000)
				performanceArgs->printPerformance(stop, durationCur);
		}

		/*
		for (int i = 0; i < searchArgs.curDepth; i++)
			std::cout << "    ";
		std::cout << "board->unmakeMove();" << std::endl;
		*/

		board->unmakeMove();
		return value;
	}

	bool maximizingPlayer = playerColor == board->curTurn;
	int bestScore = maximizingPlayer ? INT_MIN : INT_MAX;

	/*
	for (int i = 0; i < searchArgs.curDepth; i++)
		std::cout << "    ";
	std::cout << "{" << std::endl;
	*/

	std::vector<LegalMove> legalMoves = board->getLegalMoves(board->curTurn);
	for (const LegalMove& move : legalMoves) {
		int value = minimax(board, move, playerColor, searchArgs, performanceArgs, debug);

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

	/*
	for (int i = 0; i < searchArgs.curDepth; i++)
		std::cout << "    ";
	std::cout << "}" << std::endl;

	for (int i = 0; i < searchArgs.curDepth; i++)
		std::cout << "    ";
	std::cout << "board->unmakeMove();" << std::endl;
	*/

	board->unmakeMove();
	return bestScore;
}

int AI::evaluate(Board* board, PieceColor playerColor)
{
	int score = board->matEval[White] - board->matEval[Black];
	score += board->mobilityEval[White] - board->mobilityEval[Black];

	int curPhase = board->curPhase;
	if (curPhase > Evaluation::startPhase)
		curPhase = Evaluation::startPhase;

	int mgScore = board->mg_pcsqEval[White] - board->mg_pcsqEval[Black];
	int egScore = board->eg_pcsqEval[White] - board->eg_pcsqEval[Black];

	score += (curPhase * mgScore + (Evaluation::startPhase - curPhase) * egScore) / Evaluation::startPhase;

	if (playerColor == Black)
		score *= -1;

	return score;
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
