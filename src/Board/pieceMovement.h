#ifndef PIECEMOVEMENT_H
#define PIECEMOVEMENT_H

#include "piece.h"

class PieceMovement {
public:
    PieceMovement(Piece* piece, Movement* movement);

    Piece* piece;
    Movement* movement;
    bool operator==(const PieceMovement& obj2) const;
};

#endif