#ifndef TRANSPOSITIONS_H
#define TRANSPOSITIONS_H

#include <unordered_map>
#include <mutex>
#include "board.h"

struct Transposition {
    LegalMove bestMove;
    int value = 0;
    int depth = 0;
};

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
