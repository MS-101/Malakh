#include "ai.h"
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>

legalMove AI::calculateBestMove(Board* board, int depth)
{
	legalMove bestMove;

	bestMove.x1 = -1;
	bestMove.y1 = -1;
	bestMove.x2 = -1;
	bestMove.y2 = -1;
	bestMove.promotionType = Pawn;

	if (depth <= 0)
		return bestMove;

	PieceColor playerColor = board->curTurn;

	int alpha = INT_MIN;
	int beta = INT_MAX;
	int max = INT_MIN;
	

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	int positionsTotal = 0;
	long long durationTotal = 0;
	std::vector<legalMove> legalMoves = board->getLegalMoves(playerColor);
	for (const legalMove& move : legalMoves)
	{
		minimaxResponse response = minimax(board, move, playerColor, depth - 1, alpha, beta, start, positionsTotal, durationTotal);

		if (response.value > max)
		{
			max = response.value;
			bestMove = move;
		}

		start = response.start;
		positionsTotal = response.positionsTotal;
		durationTotal = response.durationTotal;
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	long long durationCur = duration.count();
	durationTotal += duration.count();
	int positionsCur = positionsTotal % positionsPerDebugMessage;

	std::cout << "Thread " << std::this_thread::get_id() << " explored " << positionsCur << " positions (total " << positionsTotal << " positions) in " << durationCur << " ms (total " << durationTotal << " ms)." << std::endl;

	return bestMove;
}

legalMove AI::calculateBestMove_threads(Board* board, int depth, int workerCount)
{
	legalMove bestMove;

	bestMove.x1 = -1;
	bestMove.y1 = -1;
	bestMove.x2 = -1;
	bestMove.y2 = -1;
	bestMove.promotionType = Pawn;

	if (depth <= 0)
		return bestMove;

	PieceColor playerColor = board->curTurn;

	int alpha = INT_MIN;
	int beta = INT_MAX;
	int max = INT_MIN;
	std::mutex maxMutex;
	
	// create move queue
	std::vector<legalMove> legalMoves = board->getLegalMoves(playerColor);
	std::mutex moveQueueMutex;
	std::queue<legalMove> moveQueue;
	for (const legalMove& move : board->getLegalMoves(playerColor))
		moveQueue.push(move);

	// create worker threads
	std::vector<std::pair<int, legalMove>> bestMoves;
	bestMoves.assign(workerCount, std::make_pair(INT_MIN, bestMove));
	std::vector<std::thread> threads;
	for (int i = 0; i < workerCount; i++) {
		threads.emplace_back([&, i]() {
			Board* localBoard = new Board(board);

			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			int positionsTotal = 0;
			long long durationTotal = 0;

			while (true) {
				moveQueueMutex.lock();

				if (moveQueue.empty()) {
					moveQueueMutex.unlock();
					break;
				}

				legalMove move = moveQueue.front();
				moveQueue.pop();

				moveQueueMutex.unlock();

				minimaxResponse response = minimax(localBoard, move, playerColor, depth - 1, alpha, beta, start, positionsTotal, durationTotal);

				start = response.start;
				positionsTotal = response.positionsTotal;
				durationTotal = response.durationTotal;

				std::pair<int, legalMove> myBestMove = bestMoves[i];
				if (response.value > myBestMove.first)
					bestMoves[i] = std::make_pair(response.value, move);
			}

			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

			long long durationCur = duration.count();
			durationTotal += duration.count();
			int positionsCur = positionsTotal % positionsPerDebugMessage;

			std::cout << "Thread " << std::this_thread::get_id() << " explored " << positionsCur << " positions (total " << positionsTotal << " positions) in " << durationCur << " ms (total " << durationTotal << " ms)." << std::endl;
			delete localBoard;
		});
	}

	// perform tasks
	for (std::thread& thread : threads)
		thread.join();

	for (std::pair<int, legalMove>& curBestMove : bestMoves) {
		if (curBestMove.first > max) {
			max = curBestMove.first;
			bestMove = curBestMove.second;
		}
	}

	return bestMove;
}

minimaxResponse AI::minimax(Board* board, legalMove move, PieceColor playerColor, int depth,  int alpha, int beta, std::chrono::high_resolution_clock::time_point start, int positionsTotal, long long durationTotal)
{
	board->makeMove(move.x1, move.y1, move.x2, move.y2, move.promotionType);
	
	if (depth == 0)
	{
		int value = evaluate(board, playerColor);
		positionsTotal++;

		if (positionsTotal % positionsPerDebugMessage == 0)
		{
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

			long long durationCur = duration.count();
			durationTotal += duration.count();
			start = stop;

			std::cout << "Thread " << std::this_thread::get_id() << " explored " << positionsPerDebugMessage << " positions (total " << positionsTotal << " positions) in " << durationCur << " ms (total " << durationTotal << " ms)." << std::endl;
		}

		board->unmakeMove();
		return minimaxResponse{ value, start, positionsTotal, durationTotal };
	}

	bool maximizingPlayer = playerColor == board->curTurn;
	int bestScore = maximizingPlayer ? INT_MIN : INT_MAX;

	std::vector<legalMove> legalMoves = board->getLegalMoves(board->curTurn);
	for (const legalMove& move : legalMoves)
	{
		minimaxResponse response = minimax(board, move, playerColor, depth - 1, alpha, beta, start, positionsTotal, durationTotal);
		start = response.start;
		positionsTotal = response.positionsTotal;
		durationTotal = response.durationTotal;

		if (maximizingPlayer)
		{
			bestScore = std::max(response.value, bestScore);
			alpha = std::max(alpha, bestScore);
		}
		else
		{
			bestScore = std::min(response.value, bestScore);
			beta = std::min(beta, bestScore);
		}

		if (beta <= alpha)
			break;
	}

	board->unmakeMove();
	return minimaxResponse{ bestScore, start, positionsTotal, durationTotal };
}

int AI::evaluate(Board* board, PieceColor playerColor)
{
	int queenWeight = 9;
	int rookWeight = 5;
	int bishopWeight = 3;
	int knightWeight = 3;
	int pawnWeight = 1;
	int mobilityWeight = 0.1;

	int whiteQueenCount = 0;
	int whiteRookCount = 0;
	int whiteBishopCount = 0;
	int whiteKnightCount = 0;
	int whitePawnCount = 0;

	std::list<Piece*> whitePieces = board->whitePieces;
	for (auto whitePieceIterator = whitePieces.begin();
		whitePieceIterator != whitePieces.end(); ++whitePieceIterator) {
		Piece* whitePiece = *whitePieceIterator;
		switch (whitePiece->type)
		{
		case::Queen:
			whiteQueenCount++;
			break;
		case::Rook:
			whiteRookCount++;
			break;
		case::Bishop:
			whiteBishopCount++;
			break;
		case::Knight:
			whiteKnightCount++;
			break;
		case::Pawn:
			whitePawnCount++;
			break;
		}
	}

	std::vector<legalMove> whiteMoves = board->getLegalMoves(White);
	int whiteMobilityCount = whiteMoves.size();

	int blackQueenCount = 0;
	int blackRookCount = 0;
	int blackBishopCount = 0;
	int blackKnightCount = 0;
	int blackPawnCount = 0;

	std::list<Piece*> blackPieces = board->blackPieces;
	for (auto blackPieceIterator = blackPieces.begin();
		blackPieceIterator != blackPieces.end(); ++blackPieceIterator) {
		Piece* blackPiece = *blackPieceIterator;
		switch (blackPiece->type)
		{
		case::Queen:
			blackQueenCount++;
			break;
		case::Rook:
			blackRookCount++;
			break;
		case::Bishop:
			blackBishopCount++;
			break;
		case::Knight:
			blackKnightCount++;
			break;
		case::Pawn:
			blackPawnCount++;
			break;
		}
	}

	std::vector<legalMove> blackMoves = board->getLegalMoves(Black);
	int blackMobilityCount = blackMoves.size();

	int materialScore = 0;
	materialScore += queenWeight * (whiteQueenCount - blackQueenCount);
	materialScore += rookWeight * (whiteRookCount - blackRookCount);
	materialScore += bishopWeight * (whiteBishopCount - blackBishopCount);
	materialScore += knightWeight * (whiteKnightCount - blackKnightCount);
	materialScore += pawnWeight * (whitePawnCount - blackPawnCount);

	int mobilityScore = 0;
	mobilityScore += mobilityWeight * (whiteMobilityCount - blackMobilityCount);

	int score = materialScore + mobilityScore;
	if (playerColor == Black)
		score *= -1;

	return score;
}
