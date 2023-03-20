#ifndef SQUARE_H
#define SQUARE_H

#include "pieceMovement.h"

#include <list>

class Square {
    public:
        Square(int x, int y);

        Piece* occupyingPiece = (Piece*)nullptr;
        std::list<PieceMovement*> movements;
        int x, y;
};

#endif