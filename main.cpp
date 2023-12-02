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
    FILE* myFile = freopen("output.txt", "w", stdout);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    ai->calculateBestMove(board, 4);

    _CrtDumpMemoryLeaks();

    /*
    board->performMove(2, 6, 2, 5, Pawn); // white move
    board->performMove(3, 1, 3, 3, Pawn); // black move
    board->performMove(3, 7, 1, 5, Pawn); // white move
    board->performMove(3, 0, 3, 2, Pawn); // black move
    board->performMove(1, 5, 1, 3, Pawn); // white move
    */

    return 0;
}
