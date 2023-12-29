#include "board.h"

void Board::initBoard(EssenceConfig essenceConfig)
{
	this->essenceConfig = essenceConfig;

	whitePawns.value = 0;
	blackPawns.value = 0;
	for (int i = 0; i < 8; i++) {
		whitePawns.setBit(i, 1);
		blackPawns.setBit(i, 6);
	}

	whiteRooks.value = 0;
	whiteRooks.setBit(0, 0);
	whiteRooks.setBit(7, 0);
	blackRooks.value = 0;
	blackRooks.setBit(0, 7);
	blackRooks.setBit(7, 7);

	whiteKnights.value = 0;
	whiteKnights.setBit(1, 0);
	whiteKnights.setBit(6, 0);
	blackKnights.value = 0;
	blackKnights.setBit(1, 7);
	blackKnights.setBit(6, 7);

	whiteBishops.value = 0;
	whiteBishops.setBit(2, 0);
	whiteBishops.setBit(5, 0);
	blackBishops.value = 0;
	blackBishops.setBit(2, 7);
	blackBishops.setBit(5, 7);

	whiteQueens.value = 0;
	whiteQueens.setBit(3, 0);
	blackQueens.value = 0;
	blackQueens.setBit(3, 7);

	whiteKings.value = 0;
	whiteKings.setBit(4, 0);
	blackKings.value = 0;
	blackKings.setBit(4, 7);

	refreshAggregations();
	notMoved.value = allPieces.value;
	whiteAttacks.value = 0;
	blackAttacks.value = 0;

	MoveGenerator::generateMoves(*this);
}

void Board::refreshAggregations()
{
	whitePieces.value = whitePawns.value | whiteRooks.value | whiteKnights.value | whiteBishops.value | whiteQueens.value | whiteKings.value;
	blackPieces.value = blackPawns.value | blackRooks.value | blackKnights.value | blackBishops.value | blackQueens.value | blackKings.value;
	allPieces.value = whitePieces.value | blackPieces.value;
}

void Board::makeMove(char x1, char y1, char x2, char y2)
{
	if (whitePawns.getBit(x1, y1)) {
		whitePawns.clearBit(x1, y1);
		whitePawns.setBit(x2, y2);
	}

	if (blackPawns.getBit(x1, y1)) {
		blackPawns.clearBit(x1, y1);
		blackPawns.setBit(x2, y2);
	}

	if (whiteRooks.getBit(x1, y1)) {
		whiteRooks.clearBit(x1, y1);
		whiteRooks.setBit(x2, y2);
	}

	if (blackRooks.getBit(x1, y1)) {
		blackRooks.clearBit(x1, y1);
		blackRooks.setBit(x2, y2);
	}

	if (whiteKnights.getBit(x1, y1)) {
		whiteKnights.clearBit(x1, y1);
		whiteKnights.setBit(x2, y2);
	}

	if (blackKnights.getBit(x1, y1)) {
		blackKnights.clearBit(x1, y1);
		blackKnights.setBit(x2, y2);
	}

	if (whiteQueens.getBit(x1, y1)) {
		whiteQueens.clearBit(x1, y1);
		whiteQueens.setBit(x2, y2);
	}

	if (blackQueens.getBit(x1, y1)) {
		blackQueens.clearBit(x1, y1);
		blackQueens.setBit(x2, y2);
	}

	if (whiteKings.getBit(x1, y1)) {
		whiteKings.clearBit(x1, y1);
		whiteKings.setBit(x2, y2);
	}

	if (blackKings.getBit(x1, y1)) {
		blackKings.clearBit(x1, y1);
		blackKings.setBit(x2, y2);
	}

	refreshAggregations();
}

std::list<LegalMove> Board::getLegalMoves(PieceColor color)
{
	if (color == White)
		return whiteMoves;
	else if (color == Black)
		return blackMoves;
}
