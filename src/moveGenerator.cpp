#include "board.h"
#include <iostream>

void MoveGenerator::generateMoves(Board* board)
{
	board->whiteAttacks.value = 0;
	board->blackAttacks.value = 0;
	board->whiteMoves.clear();
	board->blackMoves.clear();

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board->whitePawns.getBit(x, y)) {
				generateMoves(board, Pawn, White, x, y);
			} else if (board->whiteRooks.getBit(x, y)) {
				generateMoves(board, Rook, White, x, y);
			} else if (board->whiteKnights.getBit(x, y)) {
				generateMoves(board, Knight, White, x, y);
			} else if (board->whiteBishops.getBit(x, y)) {
				generateMoves(board, Bishop, White, x, y);
			} else if (board->whiteQueens.getBit(x, y)) {
				generateMoves(board, Queen, White, x, y);
			} else if (board->whiteKings.getBit(x, y)) {
				generateMoves(board, King, White, x, y);
			} else if (board->blackPawns.getBit(x, y)) {
				generateMoves(board, Pawn, Black, x, y);
			} else if (board->blackRooks.getBit(x, y)) {
				generateMoves(board, Rook, Black, x, y);
			} else if (board->blackKnights.getBit(x, y)) {
				generateMoves(board, Knight, Black, x, y);
			} else if (board->blackBishops.getBit(x, y)) {
				generateMoves(board, Bishop, Black, x, y);
			} else if (board->blackQueens.getBit(x, y)) {
				generateMoves(board, Queen, Black, x, y);
			} else if (board->blackKings.getBit(x, y)) {
				generateMoves(board, King, Black, x, y);
			}
		}
	}
}

void MoveGenerator::generateMoves(Board* board, PieceType type, PieceColor color, char x, char y)
{
	for (Mobility& mobility : board->getMobilities(type, color)) {
		int prevX = x;
		int prevY = y;
		int curX = prevX;
		int curY = prevY;
		int moveCounter = 0;
		
		if (color == White) {
			curX += mobility.start_x;
			curY += mobility.start_y;
		} else {
			curX -= mobility.start_x;
			curY -= mobility.start_y;
		}

		while ((mobility.limit == 0 || moveCounter < mobility.limit) && curX >= 0 && curX < 8 && curY >= 0 && curY < 8) {
			LegalMove legalMove{};
			legalMove.x1 = prevX;
			legalMove.y1 = prevY;
			legalMove.x2 = curX;
			legalMove.y2 = curY;
			legalMove.mobility = mobility;

			if (mobility.type == Attack || mobility.type == AttackMove) {
				if (color == White)
					board->whiteAttacks.setBit(curX, curY);
				else
					board->blackAttacks.setBit(curX, curY);
			}

			if (auto piece = board->allPieces.getBit(curX, curY)) {
				if (mobility.type == Attack || mobility.type == AttackMove) {
					if (color == White && board->blackPieces.value & piece) {
						board->whiteMoves.push_back(legalMove);
					} else if (color == Black && board->whitePieces.value & piece) {
						board->blackMoves.push_back(legalMove);
					}
				}

				break;
			} else {
				if (mobility.type == Move || mobility.type == AttackMove) {
					if (color == White) {
						board->whiteMoves.push_back(legalMove);
					} else {
						board->blackMoves.push_back(legalMove);
					}
				}
			}

			prevX = curX;
			prevY = curY;

			if (color == White) {
				curX += mobility.direction_x;
				curY += mobility.direction_y;
			} else {
				curX -= mobility.direction_x;
				curY -= mobility.direction_y;
			}

			moveCounter++;
		}
	}
}

void LegalMove::printMove() {
	char col1 =  'a' + x1;
	char row1 = '0' + 8 - y1;
	char col2 =  'a' + x2;
	char row2 =  '0' + 8 - y2;

	std::cout << col1 << row1 << col2 << row2;
}