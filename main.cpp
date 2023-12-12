#include "src/uci.h"
#include <iostream>

int main() {
    initZobrist();

    /*
    uci* api = new uci();
    api->run();
    */

    Board* board = new Board(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    AI* ai = new AI();

    LegalMove bestMove = ai->calculateBestMove(board, 4, true);
    /*
    board->makeMove(3, 6, 3, 5, Pawn);
    board->makeMove(3, 1, 3, 2, Pawn);
    board->makeMove(4, 6, 4, 5, Pawn);
    board->makeMove(4, 1, 4, 2, Pawn);

    unsigned long long hash1 = board->hash;

    board->unmakeMove();
    board->unmakeMove();
    board->unmakeMove();
    board->unmakeMove();
    board->makeMove(4, 6, 4, 5, Pawn);
    board->makeMove(4, 1, 4, 2, Pawn);
    board->makeMove(3, 6, 3, 5, Pawn);
    board->makeMove(3, 1, 3, 2, Pawn);
    
    unsigned long long hash2 = board->hash;

    board->unmakeMove();
    board->unmakeMove();
    board->unmakeMove();
    board->unmakeMove();

    if (hash1 == hash2)
        std::cout << "I am smart." << std::endl;
    else
        std::cout << "I am stupid." << std::endl;

    LegalMove bestMove = ai->calculateBestMove(board, 4, true);
    */

    //FILE* myFile = freopen("output.txt", "w", stdout);

    return 0;
}
