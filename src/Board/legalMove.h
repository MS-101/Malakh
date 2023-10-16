#ifndef LEGALMOVE_H
#define LEGALMOVE_H

#include "pieceMovement.h"

class LegalMove {
public:
	LegalMove(PieceMovement* pieceMovement, PieceType promotionType);

	PieceMovement* pieceMovement;
	PieceType promotionType;
};

#endif