#ifndef SQUARE_H
#define SQUARE_H

#include "pieceMovement.h"

class Square {
public:
    Square(int x, int y);
    ~Square();

    Piece* occupyingPiece = nullptr;
    std::list<PieceMovement*> movements;
    std::list<PieceMovement*> virtualMovements;
    int x, y;
};

#endif