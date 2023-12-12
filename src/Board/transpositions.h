#ifndef TRANSPOSITIONS_H
#define TRANSPOSITIONS_H

#include <unordered_map>
#include <mutex>
#include "piece.h"

struct LegalMove {
    int x1, y1, x2, y2;
    PieceType promotionType;
    Mobility* mobility;
};

struct Transposition {
    LegalMove bestMove{};
    int value = 0;
    int depth = -1;
};

struct szobrist {
    // 6 piece types, 2 colors, 2 states (hasMoved), 3 essences, 64 squares
    unsigned long long squares[6][2][2][3][64];
    unsigned long long turn;
} extern zobrist;

unsigned long long rand64();
void initZobrist();
unsigned long long getZobrist(Piece* piece);

class TranspositionCache {
public:
    TranspositionCache(int capacity);

    Transposition get(int key);
    void put(int key, Transposition value);
private:
    std::unordered_map<int, Transposition> cache;
    std::list<int> order;
    int capacity;
    std::mutex myMutex;
};

#endif
