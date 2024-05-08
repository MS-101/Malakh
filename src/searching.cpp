#include <searching.h>
#include <iostream>
#include <thread>
#include <queue>


TranspositionCache SearchManager::cache = TranspositionCache(4096);
Ensemble SearchManager::ensemble = Ensemble();

std::pair<bool, LegalMove> SearchManager::calculateBestMove(Board board, int depth, bool useEnsemble, bool debug)
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
			int value = minimax(newBoard, playerColor, searchArgs, &performanceArgs, useEnsemble, debug);

			if (!moveFound || value > max) {
				moveFound = true;
				max = value;
				bestMove = move;
			}
		}
	}

	if (debug)
		performanceArgs.printPerformance();

	return std::make_pair(moveFound, bestMove);
}

std::pair<bool, LegalMove> SearchManager::calculateBestMove_threads(Board board, int depth, int threadCount, bool useEnsemble, bool debug)
{
	LegalMove bestMove{};
	bool moveFound = false;

	if (depth <= 0)
		return std::make_pair(moveFound, bestMove);

	int max = INT_MIN;
	std::mutex maxMutex;

	PieceColor playerColor = board.curTurn;

	SearchArgs searchArgs{};
	searchArgs.maxDepth = depth;

	// create move queue
	std::mutex moveQueueMutex;
	std::queue<LegalMove> moveQueue;
	for (const LegalMove& move : board.getLegalMoves())
		moveQueue.push(move);

	// create worker threads
	std::vector<bool> movesFound;
	movesFound.assign(threadCount, false);

	std::vector<std::pair<int, LegalMove>> bestMoves;
	bestMoves.assign(threadCount, std::make_pair(INT_MIN, bestMove));

	std::vector<std::thread> threads;
	for (int i = 0; i < threadCount; i++) {
		threads.emplace_back([&, i]() {
			PerformanceArgs performanceArgs{};

			while (true) {
				moveQueueMutex.lock();

				if (moveQueue.empty()) {
					moveQueueMutex.unlock();
					break;
				}

				LegalMove move = moveQueue.front();
				moveQueue.pop();

				moveQueueMutex.unlock();

				Board newBoard = board;
				if (newBoard.makeMove(move)) {
					int value = minimax(newBoard, playerColor, searchArgs, &performanceArgs, useEnsemble, debug);

					std::pair<int, LegalMove> myBestMove = bestMoves[i];
					if (!movesFound[i] || value > myBestMove.first) {
						movesFound[i] = true;
						bestMoves[i] = std::make_pair(value, move);
					}
				}
			}

			if (debug)
				performanceArgs.printPerformance();
		});
	}

	// perform tasks
	for (std::thread& thread : threads)
		thread.join();

	for (int i = 0; i < threadCount; i++) {
		if (movesFound[i] && (!moveFound || bestMoves[i].first > max)) {
			moveFound = true;
			max = bestMoves[i].first;
			bestMove = bestMoves[i].second;
		}
	}

	return std::make_pair(moveFound, bestMove);
}

int SearchManager::minimax(Board board, PieceColor playerColor, SearchArgs searchArgs, PerformanceArgs* performanceArgs, bool useEnsemble, bool debug)
{
	searchArgs.curDepth++;

	/*
	Transposition transposition = cache.get(board.hash.value);
	if (transposition.depth >= searchArgs.maxDepth)
		return transposition.value;
	*/

	if (searchArgs.curDepth >= searchArgs.maxDepth && (board.isQuiet() || searchArgs.curDepth >= searchArgs.maxDepth + qLimit)) {
		int value;
		if (useEnsemble) {
			int ensembleScore = std::round(ensemble.forward(board.inputArray, board.essenceCounts));
		} else {
			value = board.evalBoard(playerColor);
		}

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
		transposition.value = value;
		transposition.depth = searchArgs.maxDepth;
		cache.put(board.hash.value, transposition);
		*/

		return value;
	}

	bool maximizingPlayer = playerColor == board.curTurn;
	int bestScore = maximizingPlayer ? INT_MIN : INT_MAX;

	for (const LegalMove& move : board.moves[board.curTurn]) {
		Board newBoard = board;
		if (newBoard.makeMove(move)) // performed move was not pseudolegal
		{
			int value = minimax(newBoard, playerColor, searchArgs, performanceArgs, useEnsemble, debug);

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

	/*
	transposition.value = bestScore;
	transposition.depth = searchArgs.maxDepth;
	cache.put(board.hash.value, transposition);
	*/

	return bestScore;
}

std::mutex PerformanceArgs::mutex;

void PerformanceArgs::printPerformance()
{
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	long long durationCur = duration.count();

	printPerformance(stop, durationCur);
}

void PerformanceArgs::printPerformance(std::chrono::high_resolution_clock::time_point stop, long long durationCur)
{
	std::lock_guard<std::mutex> lock(mutex);

	positionsTotal += positionsCur;
	durationTotal += durationCur;

	std::cout << "Thread " << std::this_thread::get_id() << " explored "
		<< positionsCur << " positions (total " << positionsTotal << " positions) in "
		<< durationCur << " ms (total " << durationTotal << " ms)." << std::endl;

	positionsCur = 0;
	start = stop;
}
