#ifndef PIECE_H
#define PIECE_H

#include "mobility.h"

#include <string>
#include <list>

enum Owner { White = 'W', Black = 'B' };
enum Essence { Classic = 'C', Red = 'R', Blue = 'B', Yellow = 'Y' };

class Piece {
    public:
        Piece();

        std::string name = "undefined";
        char tag = 'X';
        Owner owner = White;
        Essence essence = Classic;
        std::list<Mobility> mobility;
};

#endif