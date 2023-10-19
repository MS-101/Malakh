#include "src/uci.h"
#include <iostream>

int main() {
    uci* api = new uci();
    api->Run();

    /*
    Game* newGame = new Game(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    Board* board = newGame->myBoard;

    FILE* myFile = freopen("output.txt", "w", stdout);

    board->PrintBoard();
    board->PrintMoves();

    std::cout << "Performing move...\n\n";

    board->PerformMove(1, 1, 0, 0, Rook);
    board->PrintBoard();
    board->PrintMoves();
    */

    return 0;
}
