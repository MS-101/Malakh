#include "hashing.h"

bool ZobristHashing::isInitalized = false;

unsigned long long ZobristHashing::squares[64][2][6];
unsigned long long ZobristHashing::notMoved[64];
unsigned long long ZobristHashing::ghosts[64][64];
unsigned long long ZobristHashing::turn;

void ZobristHashing::initZobrist()
{
    if (isInitalized)
        return;
    isInitalized = true;

    for (char square = 0; square < 64; square++) {
        for (char color = 0; color < 2; color++)
            for (char type = 0; type < 6; type++)
                squares[square][color][type] = rand64();

        notMoved[square] = rand64();
        for (char parent = 0; parent < 64; parent++)
            ghosts[square][parent] = rand64();
    }

    turn = rand64();
}

unsigned long long ZobristHashing::rand64()
{
    static unsigned long long next = 1;

    next = next * 1103515245 + 12345;
    return next;
}

void BoardHash::switchSquare(Piece piece, char x, char y)
{
    value ^= ZobristHashing::squares[y * 8 + x][piece.color][piece.type];
}

void BoardHash::switchNotMoved(char x, char y)
{
    value ^= ZobristHashing::notMoved[y*8+x];
}

void BoardHash::switchGhost(Ghost ghost)
{
    value ^= ZobristHashing::ghosts[ghost.y * 8 + ghost.x][ghost.parentY * 8 + ghost.parentX];
}

void BoardHash::switchTurn()
{
    value ^= ZobristHashing::turn;
}
