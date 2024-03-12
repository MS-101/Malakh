#pragma once

#include <random>
#include <vector>


class Random {
public:
	static void initSeed();
	static bool coinFlip();
	static int generateRandomNumber(int lowerBound, int upperBound);
	template <typename T>static T getRandomElement(const std::vector<T>& vec);
private:
	static std::mt19937 gen;
};

template<typename T>
inline T Random::getRandomElement(const std::vector<T>& vec)
{
	if (vec.empty()) {
		return T();
	} else {
		return vec[generateRandomNumber(0, vec.size() - 1)];
	}
}
