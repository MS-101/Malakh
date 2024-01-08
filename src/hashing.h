#pragma once

#include "piece.h"

class ZobristHashing {
public:
    static unsigned long long squares[64*2*6];
    static unsigned long long notMoved[64];
    static unsigned long long ghosts[64*64];
    static unsigned long long turn;

    static void initZobrist();
private:
    static bool isInitalized;
    static unsigned long long rand64();
};

struct BoardHash {
    unsigned long long value;

    void switchSquare(Piece piece, char x, char y);
    void switchNotMoved(char x, char y);
    void switchGhost(Ghost ghost);
    void switchTurn();
};