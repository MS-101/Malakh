#ifndef PIECE_H
#define PIECE_H

#include "movement.h"
#include <list>
#include <map>

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
    std::list<Movement*> availableMoves; 
    int x = 0, y = 0;
    bool hasMoved = false;

    std::list<Mobility*> getMobilities();
    void promote(PieceType type, Essence essence);
private:
    static std::map<PieceType, std::map<Essence, std::list<Mobility*>>> mobilities;
};

class Ghost {
public:
    Ghost(int x, int y, Piece* parent);

    int x, y;
    Piece* parent;
};

#endif