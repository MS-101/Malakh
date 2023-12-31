#include "board.h"
#include <iostream>

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

	MoveGenerator::generateMoves(this);
}

void Board::printBoard()
{
	for (int y = 7; y >= 0; y--) {
		if (y != 7)
			std::cout << "-------------------------------" << std::endl;

		for (int x = 0; x < 8; x++) {
			char color = 'X';
			char essence = 'X';
			char piece = 'X';

			if (whitePawns.getBit(x, y)) {
				color = colorChars[White];
				essence = essenceChars[essenceConfig.whitePawn];
				piece = typeChars[Pawn];
			} else if (blackPawns.getBit(x, y)) {
				color = colorChars[Black];
				essence = essenceChars[essenceConfig.blackPawn];
				piece = typeChars[Pawn];
			} else if (whiteRooks.getBit(x, y)) {
				color = colorChars[White];
				essence = essenceChars[essenceConfig.whiteRook];
				piece = typeChars[Rook];
			} else if (blackRooks.getBit(x, y)) {
				color = colorChars[Black];
				essence = essenceChars[essenceConfig.blackRook];
				piece = typeChars[Rook];
			} else if (whiteKnights.getBit(x, y)) {
				color = colorChars[White];
				essence = essenceChars[essenceConfig.whiteKnight];
				piece = typeChars[Knight];
			} else if (blackKnights.getBit(x, y)) {
				color = colorChars[Black];
				essence = essenceChars[essenceConfig.blackKnight];
				piece = typeChars[Knight];
			} else if (whiteBishops.getBit(x, y)) {
				color = colorChars[White];
				essence = essenceChars[essenceConfig.whiteBishop];
				piece = typeChars[Bishop];
			} else if (blackBishops.getBit(x, y)) {
				color = colorChars[Black];
				essence = essenceChars[essenceConfig.whitePawn];
				piece = typeChars[Bishop];
			} else if (whiteQueens.getBit(x, y)) {
				color = colorChars[White];
				essence = essenceChars[Classic];
				piece = typeChars[Queen];
			} else if (blackQueens.getBit(x, y)) {
				color = colorChars[Black];
				essence = essenceChars[Classic];
				piece = typeChars[Queen];
			} else if (whiteKings.getBit(x, y)) {
				color = colorChars[White];
				essence = essenceChars[Classic];
				piece = typeChars[King];
			} else if (blackKings.getBit(x, y)) {
				color = colorChars[Black];
				essence = essenceChars[Classic];
				piece = typeChars[King];
			}

			if (x != 0)
				std::cout << "|";
			std::cout << color << essence << piece;
		}

		std::cout << std::endl;
	}
}

void Board::refreshAggregations()
{
	whitePieces.value = whitePawns.value | whiteRooks.value | whiteKnights.value | whiteBishops.value | whiteQueens.value | whiteKings.value;
	blackPieces.value = blackPawns.value | blackRooks.value | blackKnights.value | blackBishops.value | blackQueens.value | blackKings.value;
	allPieces.value = whitePieces.value | blackPieces.value;
}

void Board::makeMove(char x1, char y1, char x2, char y2)
{
	notMoved.clearBit(x1, y1);

	if (auto piece = whitePawns.getBit(x1, y1)) {
		whitePawns.value &= ~piece;
		whitePawns.setBit(x2, y2);
	} else if (auto piece = blackPawns.getBit(x1, y1)) {
		blackPawns.value &= ~piece;
		blackPawns.setBit(x2, y2);
	} else if (auto piece = whiteRooks.getBit(x1, y1)) {
		whiteRooks.value &= ~piece;
		whiteRooks.setBit(x2, y2);
	} else if (auto piece = blackRooks.getBit(x1, y1)) {
		blackRooks.value &= ~piece;
		blackRooks.setBit(x2, y2);
	} else if (auto piece = whiteKnights.getBit(x1, y1)) {
		whiteKnights.value &= ~piece;
		whiteKnights.setBit(x2, y2);
	} else if (auto piece = blackKnights.getBit(x1, y1)) {
		blackKnights.value &= ~piece;
		blackKnights.setBit(x2, y2);
	} else if (auto piece = whiteQueens.getBit(x1, y1)) {
		whiteQueens.value &= ~piece;
		whiteQueens.setBit(x2, y2);
	} else if (auto piece = blackQueens.getBit(x1, y1)) {
		blackQueens.value &= ~piece;
		blackQueens.setBit(x2, y2);
	} else if (auto piece = whiteKings.getBit(x1, y1)) {
		whiteKings.value &= ~piece;
		whiteKings.setBit(x2, y2);
	} else if (auto piece = blackKings.getBit(x1, y1)) {
		blackKings.value &= ~piece;
		blackKings.setBit(x2, y2);
	}

	refreshAggregations();
	MoveGenerator::generateMoves(this);
}

std::vector<Mobility> Board::getMobilities(PieceType type, PieceColor color)
{
	switch (type) {
	case Pawn:
		if (color == White)
			return MobilityConfig::pawnConfig[essenceConfig.whitePawn];
		else
			return MobilityConfig::pawnConfig[essenceConfig.blackPawn];
		break;
	case Rook:
		if (color == White)
			return MobilityConfig::rookConfig[essenceConfig.whiteRook];
		else
			return MobilityConfig::rookConfig[essenceConfig.blackRook];
		break;
	case Knight:
		if (color == White)
			return MobilityConfig::knightConfig[essenceConfig.whiteKnight];
		else
			return MobilityConfig::knightConfig[essenceConfig.blackKnight];
		break;
	case Bishop:
		if (color == White)
			return MobilityConfig::bishopConfig[essenceConfig.whiteBishop];
		else
			return MobilityConfig::bishopConfig[essenceConfig.blackBishop];
		break;
	case Queen:
		return MobilityConfig::queenConfig[Classic];
		break;
	case King:
		return MobilityConfig::kingConfig[Classic];
		break;
	}

	return std::vector<Mobility>();
}

std::list<LegalMove> Board::getLegalMoves(PieceColor color)
{
	if (color == White)
		return whiteMoves;
	else if (color == Black)
		return blackMoves;
}
