#include <iostream>
#include "search.h"

int main() {
    Board board;
    board.initBoard(EssenceArgs{});

    LegalMove bestMove = SearchManager::calculateBestMove(board, 5, true);
    bestMove.printMove();

    return 0;
}
