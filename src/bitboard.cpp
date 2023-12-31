#include "bitboard.h"
#include <iostream>

unsigned long long BitBoard::getBit(char x, char y)
{
    return value & (1ull << (x + y * 8));
}

void BitBoard::setBit(char x, char y)
{
	value |= 1ull << (x + y * 8);
}

void BitBoard::clearBit(char x, char y)
{
	value &= ~(1ull << (x + y * 8));
}

void BitBoard::printBits()
{
    unsigned long long mask = 1ULL << 63;

    for (int i = 1; i <= 64; ++i) {
        std::cout << ((value & mask) ? '1' : '0');
        mask >>= 1;

        if (i % 8 == 0)
            std::cout << std::endl;
    }

    std::cout << std::endl;
}
