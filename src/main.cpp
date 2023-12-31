#include <iostream>
#include "board.h"

int main() {
    Board board;
    board.initBoard(EssenceConfig{});

    board.whiteAttacks.printBits();
    board.blackAttacks.printBits();
    board.printBoard();

    return 0;
}
