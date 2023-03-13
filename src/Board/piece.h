#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <map>

enum Owner { White, Black };
enum Essence { Classic, Red, Blue, Yellow };

class Piece {
    public:
        Piece();

        std::string name = "undefined";
        char tag = 'X';
        Owner owner = White;
        Essence essence = Classic;
};

#endif