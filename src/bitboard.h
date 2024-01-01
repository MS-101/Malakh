#pragma once

struct BitBoard {
public:
	unsigned long long value = 0;

	unsigned long long getBit(char x, char y);
	void setBit(char x, char y);
	void clearBit(char x, char y);
	void printBits();
private:
	unsigned long long getMask(char x, char y);
};