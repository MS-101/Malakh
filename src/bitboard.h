#pragma once

struct BitBoard {
public:
	unsigned long long value = 0;

	BitBoard();
	unsigned long long getBit(char x, char y);
	void setBit(char x, char y);
	void clearBit(char x, char y);
	void printBits();
	unsigned long long getKingAttack(char x, char y);
	char getTropism(char x, char y);
private:
	static bool maskInitialized;
	static unsigned long long mask[8][8];
	unsigned long long getKingPattern(unsigned long long value);
	unsigned long long getTropismPattern(unsigned long long value);
};