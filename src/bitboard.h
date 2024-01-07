#pragma once

struct BitBoard {
public:
	unsigned long long value = 0;

	unsigned long long getBit(char x, char y);
	void setBit(char x, char y);
	void clearBit(char x, char y);
	void printBits();
	bool getKingAttack(char x, char y);
	char getTropism(char x, char y);
private:
	unsigned long long getMask(char x, char y);
	unsigned long long getKingPattern(unsigned long long value);
	unsigned long long getTropismPattern(unsigned long long value);
};