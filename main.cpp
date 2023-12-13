#include "src/uci.h"
#include <iostream>

int main() {
    initZobrist();

    uci* api = new uci();
    api->run();

    /*
    Board* board = new Board(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    AI* ai = new AI();

    LegalMove bestMove = ai->calculateBestMove(board, 4, true);
    */

    //FILE* myFile = freopen("output.txt", "w", stdout);

    return 0;
}
