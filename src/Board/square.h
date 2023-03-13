#ifndef SQUARE_H
#define SQUARE_H

#include "piece.h"

class Square {
    public:
        Square();

        Piece* occupyingPiece = (Piece*)nullptr;
};

#endif