#include <iostream>
#include "uci.h"

void testEngine()
{
    ZobristHashing::initZobrist();

    Board board;
    board.initBoard({});

    board.makeMove({ 4, 1, 4, 3 }); // e2-e4
    board.makeMove(6, 7, 5, 5); // g8-f6

    auto result = SearchManager::calculateBestMove(board, 4, true);
    if (result.first)
        LegalMove bestMove = result.second;
}

void testUCI()
{
    uci api;

    api.parseCommand("uci");
    api.parseCommand("isready");
    api.parseCommand("setoption name WhitePawn value Red");
    api.parseCommand("setoption name WhiteKnight value Blue");
    api.parseCommand("setoption name WhiteBishop value Blue");
    api.parseCommand("setoption name WhiteRook value Blue");
    api.parseCommand("setoption name BlackPawn value Red");
    api.parseCommand("setoption name BlackKnight value Blue");
    api.parseCommand("setoption name BlackBishop value Blue");
    api.parseCommand("setoption name BlackRook value Blue");
    api.parseCommand("ucinewgame");
    api.parseCommand("position curpos moves e2e4");
    api.parseCommand("position curpos moves g8f6");
    api.parseCommand("go depth 4");
}

void startUCI()
{
    uci api;
    api.run();
}

int main() {
    startUCI();
    // testUCI();
    // testEngine();

    return 0;
}
