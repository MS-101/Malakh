#include "square.h"

Square::Square(int x, int y) {
    this->x = x;
    this->y = y;
}

Square::~Square()
{
    if (occupyingPiece != nullptr)
        delete occupyingPiece;

    for (PieceMovement* movement : movements)
        delete movement;
    movements.clear();

    for (PieceMovement* virtualMovement : virtualMovements)
        delete virtualMovement;
    virtualMovements.clear();
}
