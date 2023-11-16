#include "pieceMovement.h"

PieceMovement::PieceMovement(Piece* piece, Movement* movement) {
    this->piece = piece;
    this->movement = movement;
}

bool PieceMovement::operator==(const PieceMovement& obj2) const
{
    if (this->piece->x == obj2.piece->x && this->piece->y == obj2.piece->y
        && this->movement->x == obj2.movement->x && this->movement->y == obj2.movement->y)
        return true;
    else
        return false;
}
