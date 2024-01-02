#pragma once

struct BitBoard {
public:
	unsigned long long value = 0;

	BitBoard();
	unsigned long long getBit(char x, char y);
	void setBit(char x, char y);
	void clearBit(char x, char y);
	void printBits();
private:
	static bool maskInitialized;
	static unsigned long long mask[8][8];
};