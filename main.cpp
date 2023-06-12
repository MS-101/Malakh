#include <iostream>
#include "src/game.h"

int main() {
    Game* newGame = new Game();
    Board* board = newGame->myBoard;

    FILE* myFile = freopen("output.txt", "w", stdout);

    board->InitBoard(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    board->PrintBoard();
    board->PrintMoves();

    std::cout << "Performing move...\n\n";

    board->PerformMove(4, 7, 1, 7);
    //board->PerformMove(7, 0, 2, 0);
    board->PrintBoard();
    board->PrintMoves();

    return 0;
}