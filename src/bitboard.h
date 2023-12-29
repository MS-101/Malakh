#pragma once

struct BitBoard {
	unsigned long long value = 0;

	unsigned long long getBit(char x, char y);
	void setBit(char x, char y);
	void clearBit(char x, char y);
	void printBits();
};
