#include "square.h"

Square::Square(int x, int y) {
    this->x = x;
    this->y = y;
}

Square::~Square()
{
    if (occupyingPiece != nullptr)
        delete occupyingPiece;

    for (auto movementIterator = movements.begin(); movementIterator != movements.end(); ++movementIterator)
        delete *movementIterator;
    movements.clear();
}
