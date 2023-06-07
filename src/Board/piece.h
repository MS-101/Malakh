#ifndef PIECE_H
#define PIECE_H

#include "movement.h"

#include <string>
#include <list>

enum Owner { White = 'W', Black = 'B' };
enum Essence { Classic = 'C', Red = 'R', Blue = 'B' };

class Piece {
    public:
        Piece(std::string name, char tag, Owner owner, Essence essence);

        std::string name = "undefined";
        char tag = 'X';
        Owner owner = White;
        Essence essence = Classic;
        std::list<Mobility*> mobilities;
        std::list<Movement*> availableMoves; 
        int x, y;
};

#endif