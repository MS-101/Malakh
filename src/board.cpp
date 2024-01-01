#include "board.h"
#include <iostream>

void Board::initBoard(EssenceArgs essenceConfig)
{
	setEssenceConfig(essenceConfig);

	clearBoard();

	for (int i = 0; i < 8; i++) {
		addPiece(White, Pawn, i, 1);
		addPiece(Black, Pawn, i, 6);
	}

	addPiece(White, Rook, 0, 0);
	addPiece(White, Rook, 7, 0);
	addPiece(Black, Rook, 0, 7);
	addPiece(Black, Rook, 7, 7);

	addPiece(White, Knight, 1, 0);
	addPiece(White, Knight, 6, 0);
	addPiece(Black, Knight, 1, 7);
	addPiece(Black, Knight, 6, 7);

	addPiece(White, Bishop, 2, 0);
	addPiece(White, Bishop, 5, 0);
	addPiece(Black, Bishop, 2, 7);
	addPiece(Black, Bishop, 5, 7);

	addPiece(White, Queen, 3, 0);
	addPiece(Black, Queen, 3, 7);

	addPiece(White, King, 4, 0);
	addPiece(Black, King, 4, 7);

	refreshAggregations();
	notMoved.value = allPieces.value;

	curTurn = White;
	MoveGenerator::generateMoves(this);
}

void Board::setEssenceConfig(EssenceArgs essenceArgs)
{
	essenceConfig[White][Pawn] = essenceArgs.whitePawn;
	essenceConfig[White][Rook] = essenceArgs.whiteRook;
	essenceConfig[White][Knight] = essenceArgs.whiteKnight;
	essenceConfig[White][Bishop] = essenceArgs.whiteBishop;
	essenceConfig[White][Queen] = Classic;
	essenceConfig[White][King] = Classic;

	essenceConfig[Black][Pawn] = essenceArgs.blackPawn;
	essenceConfig[Black][Rook] = essenceArgs.blackRook;
	essenceConfig[Black][Knight] = essenceArgs.blackKnight;
	essenceConfig[Black][Bishop] = essenceArgs.blackBishop;
	essenceConfig[Black][Queen] = Classic;
	essenceConfig[Black][King] = Classic;
}

void Board::clearBoard()
{
	for (int color = 0; color < 2; color++) {
		for (int type = 0; type < 6; type++) {
			auto& bitboard = pieces[color][type];
			bitboard.value = 0;
		}
	}
}

void Board::printBoard()
{
	for (int y = 7; y >= 0; y--) {
		if (y != 7)
			std::cout << "-------------------------------" << std::endl;

		for (int x = 0; x < 8; x++) {
			char color = 'X';
			char essence = 'X';
			char type = 'X';

			auto result = getPiece(x, y);
			if (result.first) {
				auto piece = result.second;

				color = colorChars[piece.color];
				essence = essenceChars[piece.essence];
				type = typeChars[piece.type];
			}

			if (x != 0)
				std::cout << "|";
			std::cout << color << essence << type;
		}

		std::cout << std::endl;
	}
}

std::pair<bool, Piece> Board::getPiece(char x, char y)
{
	for (int color = 0; color < 2; color++) {
		for (int type = 0; type < 6; type++) {
			auto& bitboard = pieces[color][type];

			if (bitboard.getBit(x, y)) {
				PieceEssence essence = essenceConfig[(PieceColor)color][(PieceType)type];

				return std::make_pair(true, Piece{ (PieceColor)color, (PieceType)type, essence });
			}
		}
	}

	return std::make_pair(false, Piece{});
}

std::pair<bool, Piece> Board::removePiece(char x, char y)
{
	for (int color = 0; color < 2; color++) {
		for (int type = 0; type < 6; type++) {
			auto& bitboard = pieces[color][type];

			if (bitboard.getBit(x, y)) {
				bitboard.clearBit(x, y);
				PieceEssence essence = essenceConfig[(PieceColor)color][(PieceType)type];

				return std::make_pair(true, Piece{ (PieceColor)color, (PieceType)type, essence });
			}
		}
	}

	return std::make_pair(false, Piece{});
}

void Board::addPiece(PieceColor color, PieceType type, char x, char y)
{
	pieces[color][type].setBit(x, y);
}

void Board::refreshAggregations()
{
	colors[White].value = pieces[White][Pawn].value | pieces[White][Rook].value | pieces[White][Knight].value | pieces[White][Bishop].value | pieces[White][Queen].value | pieces[White][King].value;
	colors[Black].value = pieces[Black][Pawn].value | pieces[Black][Rook].value | pieces[Black][Knight].value | pieces[Black][Bishop].value | pieces[Black][Queen].value | pieces[Black][King].value;
	allPieces.value = colors[White].value | colors[Black].value;
}

bool Board::makeMove(LegalMove move)
{
	auto result = removePiece(move.x1, move.y1);
	if (!result.first)
		return false; // no piece was found on source square

	auto movedPiece = result.second;
	if (movedPiece.color == curTurn) {
		removePiece(move.x2, move.y2);
		addPiece(movedPiece.color, movedPiece.type, move.x2, move.y2);
	} else {
		addPiece(movedPiece.color, movedPiece.type, move.x1, move.y1);
		return false; // moved piece is not owned by current player
	}

	notMoved.clearBit(move.x1, move.y1);

	refreshAggregations();
	MoveGenerator::generateMoves(this);

	bool legal = !(pieces[curTurn][King].value & attacks[opponent[curTurn]].value);
	curTurn = opponent[curTurn];

	return legal;
}

std::list<LegalMove> Board::getLegalMoves()
{
	std::list<LegalMove> legalMoves;
	for (LegalMove& move : moves[curTurn]) {
		Board newBoard = *this;
		if (newBoard.makeMove(move))
			legalMoves.push_back(move);
	}

	moves[curTurn] = legalMoves;
	return legalMoves;
}
