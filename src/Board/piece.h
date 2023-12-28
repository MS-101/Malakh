#ifndef PIECE_H
#define PIECE_H

#include "movement.h"
#include <list>
#include <vector>
#include <unordered_map>

enum PieceType { Pawn = 0, Knight = 1, Bishop = 2, Rook = 3, Queen = 4, King = 5 };
enum PieceColor { White = 0, Black = 1 };
enum Essence { Classic = 0, Red = 1, Blue = 2 };

class Piece {
public:
    Piece(PieceType type, PieceColor color, Essence essence);
    ~Piece();

    PieceType type;
    PieceColor color;
    Essence essence;
    std::list<Movement*> movements; 
    std::list<Movement*> virtualMovements;
    int x = 0, y = 0;
    bool hasMoved = false;

    std::vector<Mobility*> getMobilities();
    void change(PieceType type, Essence essence);
    bool operator==(const Piece& obj2) const;
private:
    static std::unordered_map<PieceType, std::unordered_map<Essence, std::vector<Mobility*>>> mobilities;
};

class Ghost {
public:
    Ghost(int x, int y, Piece* parent);

    int x, y;
    Piece* parent;
};

#endif