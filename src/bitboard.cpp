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

unsigned long long BitBoard::getKingPattern(unsigned long long value)
{
    unsigned long long pattern = value;

    pattern |= value << 9;
    pattern |= value << 8;
    pattern |= value << 7;
    pattern |= value << 1;
    pattern |= value >> 1;
    pattern |= value >> 7;
    pattern |= value >> 8;
    pattern |= value >> 9;

    return pattern;
}

unsigned long long BitBoard::getKingAttack(char x, char y)
{
    return getKingPattern(value) & mask[y][x];
}

unsigned long long BitBoard::getTropismPattern(unsigned long long value)
{
    unsigned long long pattern = value;

    pattern |= value << 8;
    pattern |= value << 1;
    pattern |= value >> 1;
    pattern |= value >> 8;

    return pattern;
}

char BitBoard::getTropism(char x, char y)
{
    unsigned long long curPattern = value;
    int tropism = 0;

    if (curPattern) {
        while (!(curPattern & mask[y][x])) {
            curPattern = getTropismPattern(curPattern);

            tropism++;
        }
    }

    return tropism;
}


