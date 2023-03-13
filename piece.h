#ifndef PIECE_H
#define PIECE_H

#include "movement.h"
#include "availableMove.h"
#include <string>
#include <list>

enum Owner { white, black };

class Piece {
    public:
        std::string name;
        char label;
        Owner myOwner;
        //std::list<Movement> movements;
        //std::list<std::list<AvailableMove>> availableMoves;
        //bool hasMoved = false;
        //int x, y;
};

#endif
