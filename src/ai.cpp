#include "ai.h"
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>

LegalMove* AI::calculateBestMove(Board* board, int depth)
{
	if (depth == 0)
		return nullptr;

	PieceColor playerColor = board->curTurn;

	int alpha = INT_MIN;
	int beta = INT_MAX;
	int max = INT_MIN;
	LegalMove* bestMove = nullptr;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	int positionsTotal = 0;
	long long durationTotal = 0;
	for (LegalMove* move : board->getLegalMoves(playerColor))
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

	/*
	// create tasks
	std::mutex moveQueueMutex;
	std::queue<LegalMove*> moveQueue;
	for (LegalMove* move : board->getLegalMoves(playerColor))
		moveQueue.push(move);

	// create workers
	int workerCount = 8;
	std::vector<std::pair<int, LegalMove*>> bestMoves;
	bestMoves.assign(workerCount, std::make_pair(INT_MIN, nullptr));
	std::vector<std::thread> threads;
	
	for (int i = 0; i < workerCount; i++) {
		threads.emplace_back([&, i]() {
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			int positionsTotal = 0;
			long long durationTotal = 0;

			while (true) {
				moveQueueMutex.lock();

				if (moveQueue.empty()) {
					moveQueueMutex.unlock();
					break;
				}

				LegalMove* move = moveQueue.front();
				moveQueue.pop();

				moveQueueMutex.unlock();

				minimaxResponse response = minimax(board, move, playerColor, depth - 1, alpha, beta, start, positionsTotal, durationTotal);

				start = response.start;
				positionsTotal = response.positionsTotal;
				durationTotal = response.durationTotal;

				std::pair<int, LegalMove*> myBestMove = bestMoves[i];
				if (response.value > myBestMove.first)
					bestMoves[i] = std::make_pair(response.value, move);
			}
		});
	}
	

	// perform tasks
	for (std::thread& thread : threads)
		thread.join();

	for (std::pair<int, LegalMove*> curBestMove : bestMoves) {
		if (curBestMove.first > max) {
			max = curBestMove.first;
			bestMove = curBestMove.second;
		}
	}
	*/

	return bestMove;
}

minimaxResponse AI::minimax(Board* board, LegalMove* move, PieceColor playerColor, int depth,  int alpha, int beta, std::chrono::high_resolution_clock::time_point start, int positionsTotal, long long durationTotal)
{
	Board* newBoard = new Board(board);

	int x1 = move->pieceMovement->piece->x;
	int y1 = move->pieceMovement->piece->y;
	int x2 = move->pieceMovement->movement->x;
	int y2 = move->pieceMovement->movement->y;
	PieceType promotionType = move->promotionType;

	newBoard->performMove(x1, y1, x2, y2, promotionType);

	if (depth == 0)
	{
		int value = evaluate(newBoard, playerColor);
		positionsTotal++;

		int positionsCur = 1000;
		if (positionsTotal % positionsCur == 0)
		{
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

			long long durationCur = duration.count();
			durationTotal += duration.count();
			start = stop;

			std::cout << "Thread " << std::this_thread::get_id() << " explored " << positionsCur << " positions (total " << positionsTotal << " positions) in " << durationCur << " ms (total " << durationTotal << " ms)." << std::endl;
		}

		delete newBoard;
		return minimaxResponse{ value, start, positionsTotal, durationTotal };
	}

	bool maximizingPlayer = playerColor == newBoard->curTurn;
	int bestScore = maximizingPlayer ? INT_MIN : INT_MAX;

	std::list<LegalMove*> legalMoves = newBoard->getLegalMoves(newBoard->curTurn);
	for (LegalMove* move : legalMoves)
	{
		minimaxResponse response = minimax(newBoard, move, playerColor, depth - 1, alpha, beta, start, positionsTotal, durationTotal);
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

		delete move;
	}

	delete newBoard;
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

	std::list<LegalMove*> whiteMoves = board->getLegalMoves(White);
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

	std::list<LegalMove*> blackMoves = board->getLegalMoves(Black);
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
