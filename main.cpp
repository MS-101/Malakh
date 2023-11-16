#include "src/uci.h"
#include <iostream>

int main() {
    uci* api = new uci();
    api->run();

    /*
    Game* newGame = new Game(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    Board* board = newGame->myBoard;
    */

    /*
    AI* ai = new AI();
    LegalMove* foo = ai->minimax(board, 2);
    */

    /*
    FILE* myFile = freopen("output.txt", "w", stdout);

    board->printBoard();
    board->printMoves();
    */

    return 0;
}
