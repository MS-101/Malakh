#include <iostream>
#include "search.h"

int main() {
    Board board;
    board.initBoard(EssenceArgs{});

    // board.printBoard();

    /*
    for (LegalMove &move : board.getLegalMoves())
        move.printMove();
    */

    LegalMove bestMove = SearchManager::calculateBestMove(board, 5, true);
    bestMove.printMove();

    return 0;
}
