#ifndef PIECE_H
#define PIECE_H

#include "movement.h"
#include <list>

enum PieceType { Pawn = 'P', Knight = 'N', Bishop = 'B', Rook = 'R', Queen = 'Q', King = 'K' };
enum PieceColor { White = 'W', Black = 'B' };
enum Essence { Classic = 'C', Red = 'R', Blue = 'B' };

class Piece {
public:
    Piece(PieceType type, PieceColor color, Essence essence);
    ~Piece();

    PieceType type;
    PieceColor color;
    Essence essence;
    std::list<Mobility*> mobilities;
    std::list<Movement*> availableMoves; 
    int x, y;
    bool hasMoved = false;

    void setMobilities(PieceType type, Essence essence);
};

class Ghost {
public:
    Ghost(int x, int y, Piece* parent);

    int x, y;
    Piece* parent;
};

#endif