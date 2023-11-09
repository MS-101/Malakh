#include "ai.h"

LegalMove* AI::minimax(Board* board, int depth)
{
	if (depth == 0)
		return nullptr;

	PieceColor playerColor = board->curTurn;

	int alpha = INT_MIN;
	int beta = INT_MAX;
	int max = INT_MIN;
	LegalMove* bestMove = nullptr;
	for (LegalMove* move : board->getLegalMoves(playerColor))
	{
		int score = minimax(board, move, depth - 1, alpha, beta, playerColor);
		if (score > max)
		{
			max = score;
			bestMove = move;
		}
	}

	return bestMove;
}

int AI::minimax(Board* board, LegalMove* move, int depth, int alpha, int beta, PieceColor playerColor)
{
	Board* newBoard = new Board(board);

	int x1 = move->pieceMovement->piece->x;
	int y1 = move->pieceMovement->piece->y;
	int x2 = move->pieceMovement->movement->x;
	int y2 = move->pieceMovement->movement->y;
	PieceType promotionType = move->promotionType;

	newBoard->performMove(x1, y1, x2, y2, promotionType);

	bool maximizingPlayer = playerColor == newBoard->curTurn;
	int bestScore = maximizingPlayer ? INT_MIN : INT_MAX;

	if (depth == 0)
	{
		bestScore = evaluate(newBoard, playerColor);

		delete newBoard;
		return bestScore;
	}

	std::list<LegalMove*> legalMoves = newBoard->getLegalMoves(newBoard->curTurn);
	for (LegalMove* move : legalMoves)
	{
		int score = minimax(newBoard, move, depth - 1, alpha, beta, playerColor);
		if (maximizingPlayer)
		{
			bestScore = std::max(score, bestScore);
			alpha = std::max(alpha, bestScore);
		}
		else
		{
			bestScore = std::min(score, bestScore);
			beta = std::min(beta, bestScore);
		}

		if (beta <= alpha)
			break;
	}

	delete newBoard;
	return bestScore;
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
