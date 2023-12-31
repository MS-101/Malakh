#pragma once

#include <unordered_map>

enum PieceColor { White, Black };
enum PieceEssence { Classic, Red, Blue };
enum PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

extern std::unordered_map<PieceColor, char> colorChars;
extern std::unordered_map<PieceEssence, char> essenceChars;
extern std::unordered_map<PieceType, char> typeChars;