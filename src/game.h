#ifndef GAME_H
#define GAME_H

#include "Board/board.h"

class Game {
    public:
        Game();
        
        Board* myBoard = new Board();
};

#endif
