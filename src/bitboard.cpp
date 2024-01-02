#include "bitboard.h"
#include <iostream>

bool BitBoard::maskInitialized = false;
unsigned long long BitBoard::mask[8][8];

BitBoard::BitBoard()
{
    if (!maskInitialized) {
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                mask[y][x] = 1ull << (x + y * 8);

        maskInitialized = true;
    }
}

unsigned long long BitBoard::getBit(char x, char y)
{
    return value & mask[y][x];
}

void BitBoard::setBit(char x, char y)
{
	value |= mask[y][x];
}

void BitBoard::clearBit(char x, char y)
{
    value &= ~mask[y][x];
}

void BitBoard::printBits()
{
    for (int y = 7; y >= 0; y--) {
        for (int x = 0; x < 8; x++)
            std::cout << ((value & mask[y][x]) ? '1' : '0');

        std::cout << std::endl;
    }
}


