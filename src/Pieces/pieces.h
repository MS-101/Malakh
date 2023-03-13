#ifndef PIECES_H
#define PIECES_H

#include "../Board/piece.h"

class Pawn : public Piece {
    public:
        Pawn(Essence essence, Owner owner);
};

class Rook : public Piece {
    public:
        Rook(Essence essence, Owner owner);
};

class Knight : public Piece {
    public:
        Knight(Essence essence, Owner owner);
};

class Bishop : public Piece {
    public:
        Bishop(Essence essence, Owner owner);
};

class Queen : public Piece {
    public:
        Queen(Essence essence, Owner owner);
};

class King : public Piece {
    public:
        King(Essence essence, Owner owner);
};

#endif