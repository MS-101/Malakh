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

    auto result = SearchManager::calculateBestMove(board, 5, true);
    if (result.first)
        result.second.printMove();

    return 0;
}
