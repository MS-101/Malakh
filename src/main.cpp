#include <iostream>
#include "search.h"

int main() {
    Board board;
    board.initBoard(EssenceArgs{});

    // board.printBoard();
    // board.printMoves();

    auto result = SearchManager::calculateBestMove(board, 5, true);

    return 0;
}
