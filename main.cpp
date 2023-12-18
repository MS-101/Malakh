#include "src/uci.h"
#include <iostream>

void testEngine()
{
    Board* board = new Board(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    AI* ai = new AI();

    board->makeMove(4, 6, 4, 4); // e2-e4
    board->makeMove(4, 1, 4, 3); // e7-e5
    board->makeMove(6, 7, 5, 5); // Ng1-f3
    board->makeMove(3, 1, 3, 3); // d7-d5
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

    // FILE* myFile = freopen("output.txt", "w", stdout);
    LegalMove bestMove = ai->calculateBestMove(new Board(board), 2, true);
}

void testUCI()
{
    uci* api = new uci();

    api->parseCommand("uci");
    api->parseCommand("isready");
    api->parseCommand("setoption name WhitePawn value Classic");
    api->parseCommand("setoption name WhiteKnight value Classic");
    api->parseCommand("setoption name WhiteBishop value Classic");
    api->parseCommand("setoption name WhiteRook value Classic");
    api->parseCommand("setoption name BlackPawn value Classic");
    api->parseCommand("setoption name BlackKnight value Classic");
    api->parseCommand("setoption name BlackBishop value Classic");
    api->parseCommand("setoption name BlackRook value Classic");
    api->parseCommand("ucinewgame");
    api->parseCommand("position curpos moves e2e4");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves e7e5");
    api->parseCommand("position curpos moves g1f3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves d7d5");
    api->parseCommand("position curpos moves f3e5");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves d5e4");
    api->parseCommand("position curpos moves f1c4");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves d8d4");
    api->parseCommand("position curpos moves e5f7");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves d4c4");
    api->parseCommand("position curpos moves f7h8");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves f8c5");
    api->parseCommand("position curpos moves b1c3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves g8f6");
    api->parseCommand("position curpos moves d2d3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves e4d3");
    api->parseCommand("position curpos moves c2d3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves c4e6");
    api->parseCommand("position curpos moves d1e2");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves e6e2");
    api->parseCommand("position curpos moves c3e2");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves c8g4");
    api->parseCommand("position curpos moves c1e3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves c5b4");
    api->parseCommand("position curpos moves e2c3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves b8c6");
    api->parseCommand("position curpos moves e1g1");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves b4c3");
    api->parseCommand("position curpos moves b2c3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves a7a6");
    api->parseCommand("position curpos moves f1b1");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves c6e5");
    api->parseCommand("position curpos moves d3d4");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves e5d3");
    api->parseCommand("position curpos moves b1b7");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves f6e4");
    api->parseCommand("position curpos moves f2f3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves e4c3");
    api->parseCommand("position curpos moves b7c7");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves c3e2");
    api->parseCommand("position curpos moves g1f1");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves g4f3");
    api->parseCommand("position curpos moves c7g7");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves f3h5");
    api->parseCommand("position curpos moves g2g4");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves h5g4");
    api->parseCommand("position curpos moves h8f7");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves g4f3");
    api->parseCommand("position curpos moves d4d5");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves e8f8");
    api->parseCommand("position curpos moves f7g5");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves f8g7");
    api->parseCommand("position curpos moves g5f3");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves e2c3");
    api->parseCommand("position curpos moves e3d4");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves g7g8");
    api->parseCommand("position curpos moves f3e1");
    api->parseCommand("go depth 2");
    api->parseCommand("position curpos moves a8f8");
    api->parseCommand("position curpos moves d4f6");
    api->parseCommand("go depth 2");
}

void startUCI()
{
    uci* api = new uci();
    api->run();
}

int main() {
    initZobrist();

    startUCI();
    // testUCI();
    // testEngine();

    return 0;
}
