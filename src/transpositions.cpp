#include "transpositions.h"

TranspositionCache::TranspositionCache(int capacity)
{
    mutex = new std::mutex();
	this->capacity = capacity;
	cache = std::unordered_map<unsigned long long, Transposition>(capacity);
}

Transposition TranspositionCache::get(unsigned long long key)
{
    std::lock_guard<std::mutex> lock(*mutex);

    if (cache.find(key) != cache.end()) {
        order.remove(key);
        order.push_front(key);

        return cache[key];
    }

    return Transposition{};
}

void TranspositionCache::put(unsigned long long key, Transposition value)
{
    std::lock_guard<std::mutex> lock(*mutex);

    if (cache.find(key) != cache.end()) {
        order.remove(key);
        order.push_front(key);
    } else if (cache.size() >= capacity) {
        unsigned long long removedKey = order.back();
        order.pop_back();
        cache.erase(removedKey);
    }

    cache[key] = value;
    order.push_front(key);
}
