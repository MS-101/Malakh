#include "src/uci.h"
#include <iostream>

int main() {
    initZobrist();

    uci* api = new uci();
    api->run();

    /*
    Board* board = new Board(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    AI* ai = new AI();
    */

    /*
    FILE* myFile = freopen("output.txt", "w", stdout);
    LegalMove bestMove = ai->calculateBestMove(board, 2, false);
    */

    return 0;
}
