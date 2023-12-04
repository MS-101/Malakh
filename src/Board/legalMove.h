#ifndef MOVE_H
#define MOVE_H

#include "piece.h"

class LegalMove {
public:
	LegalMove(int x1, int y1, int x2, int y2, PieceType promotionType);

	int x1, y1, x2, y2;
	PieceType promotionType;
};

#endif