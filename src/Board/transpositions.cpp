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
