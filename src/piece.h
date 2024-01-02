#pragma once

enum PieceColor { White, Black };
enum PieceEssence { Classic, Red, Blue };
enum PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

struct Piece {
    PieceColor color;
    PieceType type;
    PieceEssence essence;
};

extern PieceColor opponent[];
extern char colorChars[];
extern char essenceChars[];
extern char typeChars[];