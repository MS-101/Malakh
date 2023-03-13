#ifndef SQUARE_H
#define SQUARE_H

#include "piece.h"
#include <list>

class Square {
    public:
        Piece *occupyingPiece;
        std::list<AvailableMove> *availableMoves;
};

#endif