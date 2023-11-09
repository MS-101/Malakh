#include "src/uci.h"
#include <iostream>

int main() {
    uci* api = new uci();
    api->run();

    /*
    Game* newGame = new Game(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    Board* board = newGame->myBoard;
    AI* ai = new AI();

    board->performMove(1, 2, 1, 1, Pawn);

    LegalMove* bestMove = ai->minimax(board, 2);

    FILE* myFile = freopen("output.txt", "w", stdout);
    std::cout << "Performing moves...\n\n";
    board->performMove(1, 0, 3, 1, Pawn);

    board->printBoard();
    board->printMoves();
    */

    return 0;
}
