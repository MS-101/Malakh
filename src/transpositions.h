#pragma once

#include <unordered_map>
#include <mutex>

struct Transposition {
    int value = 0;
    int depth = -1;
};

class TranspositionCache {
public:
    TranspositionCache(int capacity);

    Transposition get(unsigned long long key);
    void put(unsigned long long key, Transposition value);
private:
    std::unordered_map<unsigned long long, Transposition> cache;
    std::list<unsigned long long> order;
    int capacity;
    std::mutex* mutex;
};