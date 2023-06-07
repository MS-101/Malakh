#ifndef PIECES_H
#define PIECES_H

#include "../Board/piece.h"

class Pawn : public Piece {
    public:
        Pawn(Owner owner, Essence essence);
};

class Rook : public Piece {
    public:
        Rook(Owner owner, Essence essence);
};

class Knight : public Piece {
    public:
        Knight(Owner owner, Essence essence);
};

class Bishop : public Piece {
    public:
        Bishop(Owner owner, Essence essence);
};

class Queen : public Piece {
    public:
        Queen(Owner owner);
};

class King : public Piece {
    public:
        King(Owner owner);
};

#endif