#include "transpositions.h"


TranspositionCache::TranspositionCache(int capacity)
{
	this->capacity = capacity;
    cache = std::unordered_map<int, Transposition>(capacity);
}

Transposition TranspositionCache::get(int key)
{
    std::lock_guard<std::mutex> lock(myMutex);

    if (cache.find(key) != cache.end()) {
        order.remove(key);
        order.push_front(key);

        return cache[key];
    }

    return Transposition{};
}

void TranspositionCache::put(int key, Transposition value)
{
    std::lock_guard<std::mutex> lock(myMutex);

    if (cache.size() >= capacity) {
        int removedKey = order.back();
        order.pop_back();
        cache.erase(removedKey);
    }

    cache[key] = value;
    order.push_front(key);
}

szobrist zobrist;

unsigned long long rand64()
{
    static unsigned long long next = 1;

    next = next * 1103515245 + 12345;
    return next;
}

void initZobrist()
{
    for (int type = 0; type < 6; type++)
        for (int color = 0; color < 2; color++)
            for (int hasMoved = 0; hasMoved < 2; hasMoved++)
                for (int essence = 0; essence < 3; essence++)
                    for (int square = 0; square < 64; square++) // squares
                        zobrist.squares[type][color][hasMoved][essence][square] = rand64();

    zobrist.turn = rand64();
}

unsigned long long getZobrist(Piece* piece)
{
    return zobrist.squares[piece->type][piece->color][piece->hasMoved][piece->essence][piece->y * 8 + piece->x];
}
