#include <iostream>
#include "src/game.h"

int main() {
    std::cout << "Starting chess engine!\n\n";

    Game* newGame = new Game();
    Board* board = newGame->myBoard;

    board->InitBoard(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    board->PrintBoard();
    /*
    board->MovePiece(1, 1, 1, 2);
    board->PrintBoard();
    */

    return 0;
}