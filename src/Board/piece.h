#ifndef PIECE_H
#define PIECE_H

#include "movement.h"
#include <list>
#include <unordered_map>

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
    void change(PieceType type, Essence essence);
private:
    static std::unordered_map<PieceType, std::unordered_map<Essence, std::list<Mobility*>>> mobilities;
};

class Ghost {
public:
    Ghost(int x, int y, Piece* parent);

    int x, y;
    Piece* parent;
};

#endif