#include "chess.h"

std::unordered_map<PieceColor, char> colorChars = {
	{White, 'W'},
	{Black, 'B'}
};

std::unordered_map<PieceEssence, char> essenceChars = {
	{Classic, 'C'},
	{Red, 'R'},
	{Blue, 'B'}
};

std::unordered_map<PieceType, char> typeChars = {
	{Pawn, 'P'},
	{Rook, 'R'},
	{Knight, 'N'},
	{Bishop, 'B'},
	{Queen, 'Q'},
	{King, 'K'}
};