#include <bitboard.h>
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
    value &= ~getMask(x, y);
}

void BitBoard::printBits()
{
    for (int y = 7; y >= 0; y--) {
        for (int x = 0; x < 8; x++)
            std::cout << ((value & getMask(x, y)) ? '1' : '0');

        std::cout << std::endl;
    }
}

unsigned long long BitBoard::getMask(char x, char y)
{
    return 1ull << ((7 - x) + y * 8);
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

bool BitBoard::getKingAttack(char x, char y)
{
    return getKingPattern(value) & getMask(x, y);
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
    unsigned long long pattern = value;
    int tropism = 0;

    if (pattern) {
        while (!(pattern & getMask(x, y))) {
            pattern = getTropismPattern(pattern);

            tropism++;
        }
    }

    return tropism;
}
