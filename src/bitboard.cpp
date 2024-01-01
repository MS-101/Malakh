#include "bitboard.h"
#include <iostream>

unsigned long long BitBoard::getBit(char x, char y)
{
    return value & getMask(x, y);
}

void BitBoard::setBit(char x, char y)
{
	value |= getMask(x, y);
}

void BitBoard::clearBit(char x, char y)
{
    value &= ~getMask(x, y);;
}

unsigned long long BitBoard::getMask(char x, char y)
{
    return 1ull << (x + y * 8);
}

void BitBoard::printBits()
{
    for (int y = 7; y >= 0; y--) {
        for (int x = 0; x < 8; x++)
            std::cout << ((value & getMask(x, y)) ? '1' : '0');

        std::cout << std::endl;
    }
}


