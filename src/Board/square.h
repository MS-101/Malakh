#ifndef SQUARE_H
#define SQUARE_H

#include "pieceMovement.h"

class Square {
public:
    Square(int x, int y);
    ~Square();

    Piece* occupyingPiece = nullptr;
    std::list<PieceMovement*> movements;
    int x, y;
};

#endif