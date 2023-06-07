#include <iostream>
#include "src/game.h"

int main() {
    Game* newGame = new Game();
    Board* board = newGame->myBoard;

    freopen("output.txt", "w", stdout);
    board->InitBoard(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    /*
    board->PrintBoard();
    board->PrintMoves();
    */

    board->PerformMove(5, 2, 4, 2);
    board->PrintBoard();
    board->PrintMoves();

    return 0;
}