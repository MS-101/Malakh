#include "game.h"

Game::Game(Essence whitePawnEssence, Essence whiteRookEssence, Essence whiteKnightEssence, Essence whiteBishopEssence,
    Essence blackPawnEssence, Essence blackRookEssence, Essence blackKnightEssence, Essence blackBishopEssence) {
    myBoard->InitBoard(whitePawnEssence, whiteRookEssence, whiteKnightEssence, whiteBishopEssence,
        blackPawnEssence, blackRookEssence, blackKnightEssence, blackBishopEssence);
}