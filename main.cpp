#include <iostream>
#include "src/game.h"

int main() {
    std::cout << "Starting chess engine!\n\n";

    Game* newGame = new Game();
    Board* board = newGame->myBoard;

    board->InitBoard(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    board->PrintBoard();
    board->PrintMoves();

    /*
    board->PerformMove(5, 5, 5, 3);
    board->PrintBoard();
    board->PrintMoves();
    */

    return 0;
}