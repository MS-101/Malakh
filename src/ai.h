#ifndef AI_H
#define A_H

#include "Board/board.h"

class AI {
public:
	LegalMove* minimax(Board* board, int depth);
private:
	int minimax(Board* board, LegalMove* move, int depth, int alpha, int beta, PieceColor playerColor);
	int evaluate(Board* board, PieceColor playerColor);
};

#endif
