#include <board.h>
#include <iostream>


void MoveGenerator::clearMoves(Board* board)
{
	for (int color = 0; color < 2; color++) {
		for (int type = 0; type < 6; type++)
			board->eval.mobCount[getPieceIndex(color, type)] = 0;

		board->eval.attCount[color] = 0;
		board->eval.attWeight[color] = 0;
	}

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

	char whiteKingIndex = getPieceIndex(White, King);
	char whiteRookIndex = getPieceIndex(White, Rook);
	char blackKingIndex = getPieceIndex(Black, King);
	char blackRookIndex = getPieceIndex(Black, Rook);

	// white queen-side castle
	if (board->pieces[whiteKingIndex].getBit(4, 0) && board->notMoved.getBit(4, 0) && !board->attacks[Black].getBit(4, 0)
		&& board->pieces[whiteRookIndex].getBit(0, 0) && board->notMoved.getBit(0, 0)
		&& !board->allPieces.getBit(1, 0) && !board->attacks[Black].getBit(1, 0)
		&& !board->allPieces.getBit(2, 0) && !board->attacks[Black].getBit(2, 0)
		&& !board->allPieces.getBit(3, 0) && !board->attacks[Black].getBit(3, 0)) {
		LegalMove move{};
		move.castling = queenSide;

		board->moves[White].push_back(move);
	}

	// black queen-side castle
	if (board->pieces[blackKingIndex].getBit(4, 7) && board->notMoved.getBit(4, 7) && !board->attacks[White].getBit(4, 7)
		&& board->pieces[blackRookIndex].getBit(0, 7) && board->notMoved.getBit(0, 7)
		&& !board->allPieces.getBit(1, 7) && !board->attacks[White].getBit(1, 7)
		&& !board->allPieces.getBit(2, 7) && !board->attacks[White].getBit(2, 7)
		&& !board->allPieces.getBit(3, 7) && !board->attacks[White].getBit(3, 7)) {
		LegalMove move{};
		move.castling = queenSide;

		board->moves[Black].push_back(move);
	}

	// white king-side castle
	if (board->pieces[whiteKingIndex].getBit(4, 0) && board->notMoved.getBit(4, 0) && !board->attacks[Black].getBit(4, 0)
		&& board->pieces[whiteRookIndex].getBit(7, 0) && board->notMoved.getBit(7, 0)
		&& !board->allPieces.getBit(5, 0) && !board->attacks[Black].getBit(5, 0)
		&& !board->allPieces.getBit(6, 0) && !board->attacks[Black].getBit(6, 0)) {
		LegalMove move{};
		move.castling = kingSide;

		board->moves[White].push_back(move);
	}

	// black king-side castle
	if (board->pieces[blackKingIndex].getBit(4, 7) && board->notMoved.getBit(4, 7) && !board->attacks[White].getBit(4, 7)
		&& board->pieces[blackRookIndex].getBit(7, 7) && board->notMoved.getBit(7, 7)
		&& !board->allPieces.getBit(5, 7) && !board->attacks[White].getBit(5, 7)
		&& !board->allPieces.getBit(6, 7) && !board->attacks[White].getBit(6, 7)) {
		LegalMove move{};
		move.castling = kingSide;

		board->moves[Black].push_back(move);
	}

	board->movesValidated = false;
}

void MoveGenerator::generateMoves(Board* board, Piece piece, char x, char y)
{
	bool kingAttacked = false;
	char pieceIndex = getPieceIndex(piece.color, piece.type);
	char kingIndex = getPieceIndex(opponent[piece.color], King);

	for (const Mobility& mobility : Mobilities::mobilityConfig[piece.type][piece.essence]) {
		if (mobility.flags.initiative && !board->notMoved.getBit(x, y))
			continue;

		int destinationX = x;
		int destinationY = y;
		int moveCounter = 0;
		
		switch (piece.color) {
		case White:
			destinationX += mobility.start_x;
			destinationY += mobility.start_y;
			break;
		case Black:
			destinationX -= mobility.start_x;
			destinationY -= mobility.start_y;
			break;
		}

		while ((mobility.limit == 0 || moveCounter < mobility.limit)
			&& destinationX >= 0 && destinationX < 8 && destinationY >= 0 && destinationY < 8) {
			LegalMove legalMove(x, y, destinationX, destinationY, mobility);
			bool isPromotion = (piece.type == Pawn
				&& ((piece.color == White && destinationY == 7) || (piece.color == Black && destinationY == 0)));

			if (mobility.type == Attack || mobility.type == AttackMove) {
				board->attacks[piece.color].setBit(destinationX, destinationY);

				if (board->pieces[kingIndex].getKingAttack(destinationX, destinationY)) {
					board->eval.attWeight[piece.color] += Evaluation::pieceAttWeights[piece.type];
					kingAttacked = true;
				}
			}

			if (!mobility.flags.uninterruptible || moveCounter + 1 == mobility.limit) {
				if (board->allPieces.getBit(destinationX, destinationY)) {
					if ((mobility.type == Attack || mobility.type == AttackMove)
						&& (board->colors[opponent[piece.color]].getBit(destinationX, destinationY))) {
						if (!isPromotion) {
							board->moves[piece.color].push_back(legalMove);
						} else {
							legalMove.promotion = Queen;
							board->moves[piece.color].push_back(legalMove);
							legalMove.promotion = Rook;
							board->moves[piece.color].push_back(legalMove);
							legalMove.promotion = Bishop;
							board->moves[piece.color].push_back(legalMove);
							legalMove.promotion = Knight;
							board->moves[piece.color].push_back(legalMove);
						}

						board->eval.mobCount[pieceIndex]++;
					}

					break;
				} else if (mobility.type == Move || mobility.type == AttackMove || (mobility.type == Attack
					&& mobility.flags.vigilant && board->ghost.x == destinationX && board->ghost.y == destinationY)) {
					if (!isPromotion) {
						board->moves[piece.color].push_back(legalMove);
					} else {
						legalMove.promotion = Queen;
						board->moves[piece.color].push_back(legalMove);
						legalMove.promotion = Rook;
						board->moves[piece.color].push_back(legalMove);
						legalMove.promotion = Bishop;
						board->moves[piece.color].push_back(legalMove);
						legalMove.promotion = Knight;
						board->moves[piece.color].push_back(legalMove);
					}

					board->eval.mobCount[pieceIndex]++;
				}
			}

			switch (piece.color) {
			case White:
				destinationX += mobility.direction_x;
				destinationY += mobility.direction_y;
				break;
			case Black:
				destinationX -= mobility.direction_x;
				destinationY -= mobility.direction_y;
				break;
			}

			moveCounter++;
		}
	}

	if (kingAttacked) // if this piece generated an attack on king increment attacking piece counter
		board->eval.attCount[piece.color]++;
}
