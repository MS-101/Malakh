#include "src/uci.h"
#include <iostream>

void testEngine()
{
    initZobrist();

    Board* board = new Board(EssenceConfig{});
    AI* ai = new AI();

    board->makeMove(4, 6, 4, 4); // e2-e4
    board->makeMove(4, 1, 4, 3); // e7-e5
    board->makeMove(6, 7, 5, 5); // Ng1-f3
    board->makeMove(3, 1, 3, 3); // d7-d5

    LegalMove bestMove = ai->calculateBestMove(new Board(board), 3, true);

    /*
    board->makeMove(5, 5, 4, 3); // Nf3-e5
    board->makeMove(3, 3, 4, 4); // d5xe4
    board->makeMove(5, 7, 2, 4); // Bf1-c4
    board->makeMove(3, 0, 3, 4); // Qd8-d4
    board->makeMove(4, 3, 5, 1); // Ne5-f7
    board->makeMove(3, 4, 2, 4); // Qd4xc4
    board->makeMove(5, 1, 7, 0); // Nf7-h8
    board->makeMove(5, 0, 2, 3); // Bf8-c5
    board->makeMove(1, 7, 2, 5); // Nb1-c3
    board->makeMove(6, 0, 5, 2); // Ng8-f6
    board->makeMove(3, 6, 3, 5); // d2-d3
    board->makeMove(4, 4, 3, 5); // e4xd3
    board->makeMove(2, 6, 3, 5); // c2-d3
    board->makeMove(2, 4, 4, 2); // Qc4-e6
    board->makeMove(3, 7, 4, 6); // Qd1-e2
    board->makeMove(4, 2, 4, 6); // Qe6xe2
    board->makeMove(2, 5, 4, 6); // Nc3-e2
    board->makeMove(2, 0, 6, 4); // Bc8-g4
    board->makeMove(2, 7, 4, 5); // Bc1-e3
    board->makeMove(2, 3, 1, 4); // Bc5-b4
    board->makeMove(4, 6, 2, 5); // Ne2-c3
    board->makeMove(1, 0, 2, 2); // Nb8-c6
    board->makeMove(4, 7, 6, 7); // Ke1-g1
    board->makeMove(1, 4, 2, 5); // Bb4xc3
    board->makeMove(1, 6, 2, 5); // b2-c3
    */

    // FILE* myFile = freopen("output.txt", "w", stdout);
    
}

void testUCI()
{
    uci* api = new uci();

    api->parseCommand("uci");
    api->parseCommand("isready");
    api->parseCommand("setoption name WhitePawn value Red");
    api->parseCommand("setoption name WhiteKnight value Blue");
    api->parseCommand("setoption name WhiteBishop value Blue");
    api->parseCommand("setoption name WhiteRook value Blue");
    api->parseCommand("setoption name BlackPawn value Red");
    api->parseCommand("setoption name BlackKnight value Blue");
    api->parseCommand("setoption name BlackBishop value Blue");
    api->parseCommand("setoption name BlackRook value Blue");
    api->parseCommand("ucinewgame");
    api->parseCommand("position curpos moves e2e4");
    api->parseCommand("position curpos moves g8f6");

    // FILE* myFile = freopen("output.txt", "w", stdout);
    api->parseCommand("go depth 2");
}

void startUCI()
{
    uci* api = new uci();
    api->run();
}

int main() {
    startUCI();
    // testUCI();
    // testEngine();

    return 0;
}
