#ifndef PIECES_H
#define PIECES_H

#include "../Board/piece.h"

class Pawn : public Piece {
    public:
        Pawn(Essence essence, Owner owner);
};

#endif