#include "board.h"
#include <iostream>

void MoveGenerator::clearMoves(Board* board)
{
	for (int color = 0; color < 2; color++)
		for (int type = 0; type < 6; type++)
			board->eval.mobCounts[color][type] = 0;

	board->attacks[White].value = 0;
	board->attacks[Black].value = 0;
	board->moves[White].clear();
	board->moves[Black].clear();
}

void MoveGenerator::generateMoves(Board* board)
{
	clearMoves(board);

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			auto result = board->getPiece(x, y);

			if (result.first) {
				auto piece = result.second;
				generateMoves(board, piece, x, y);
			}
		}
	}
}

void MoveGenerator::generateMoves(Board* board, Piece piece, char x, char y)
{
	for (Mobility& mobility : mobilityConfig[piece.type][piece.essence]) {
		int destinationX = x;
		int destinationY = y;
		int moveCounter = 0;
		
		if (piece.color == White) {
			destinationX += mobility.start_x;
			destinationY += mobility.start_y;
		} else {
			destinationX -= mobility.start_x;
			destinationY -= mobility.start_y;
		}

		while ((mobility.limit == 0 || moveCounter < mobility.limit) && destinationX >= 0 && destinationX < 8 && destinationY >= 0 && destinationY < 8) {
			LegalMove legalMove{};
			legalMove.x1 = x;
			legalMove.y1 = y;
			legalMove.x2 = destinationX;
			legalMove.y2 = destinationY;
			legalMove.mobility = mobility;

			if (mobility.type == Attack || mobility.type == AttackMove)
				board->attacks[piece.color].setBit(destinationX, destinationY);

			if (board->allPieces.getBit(destinationX, destinationY)) {
				if (mobility.type == Attack || mobility.type == AttackMove) {
					if (board->colors[opponent[piece.color]].getBit(destinationX, destinationY)) {
						board->moves[piece.color].push_back(legalMove);
						board->eval.mobCounts[piece.color][piece.type]++;
					}
				}

				break;
			} else {
				if (mobility.type == Move || mobility.type == AttackMove) {
					board->moves[piece.color].push_back(legalMove);
					board->eval.mobCounts[piece.color][piece.type]++;
				}
			}

			if (piece.color == White) {
				destinationX += mobility.direction_x;
				destinationY += mobility.direction_y;
			} else {
				destinationX -= mobility.direction_x;
				destinationY -= mobility.direction_y;
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

	std::cout << col1 << row1 << col2 << row2 << std::endl;
}