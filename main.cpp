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

    board->PerformMove(0, 7, 0, 5);
    board->PrintBoard();
    board->PrintMoves();

    std::cout << "Performing move...\n\n";

    board->PerformMove(1, 5, 0, 6);
    board->PrintBoard();
    board->PrintMoves();

    return 0;
}