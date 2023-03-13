#include <iostream>
#include "src/game.h"

int main() {
    std::cout << "Starting chess engine!\n\n";

    Game newGame;
    newGame.myBoard.PrintBoard();

    return 0;
}