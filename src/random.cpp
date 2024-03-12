#include <random.h>


std::mt19937 Random::gen;

void Random::initSeed()
{
	std::random_device rd;
	gen = std::mt19937(rd());
}

bool Random::coinFlip()
{
	return generateRandomNumber(0, 1);
}

int Random::generateRandomNumber(int lowerBound, int upperBound)
{
	std::uniform_int_distribution<int> distribution(lowerBound, upperBound);

	return distribution(gen);
}
