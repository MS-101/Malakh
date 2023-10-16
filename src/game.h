#ifndef GAME_H
#define GAME_H

#include "Board/board.h"

class Game {
public:
    Game(Essence whitePawnEssence, Essence whiteRookEssence, Essence whiteKnightEssence, Essence whiteBishopEssence,
        Essence blackPawnEssence, Essence blackRookEssence, Essence blackKnightEssence, Essence blackBishopEssence);

    Board* myBoard = new Board();
};

#endif
