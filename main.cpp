#include "src/uci.h"
#include "src/ai.h"
#include <iostream>

int main() {
    /*
    uci* api = new uci();
    api->run();
    */

    AI* ai = new AI();
    Game* newGame = new Game(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    Board* board = newGame->myBoard;

    LegalMove bestMove = ai->calculateBestMove(board, 4, true);


    //FILE* myFile = freopen("output.txt", "w", stdout);

    return 0;
}
