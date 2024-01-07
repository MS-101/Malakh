#pragma once

enum PieceColor { White, Black };
enum PieceEssence { Classic, Red, Blue };
enum PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

struct Piece {
    PieceColor color = White;
    PieceType type = Pawn;
    PieceEssence essence = Classic;
};

struct Ghost {
    int x = -1;
    int y = -1;
    int parentX = -1;
    int parentY = -1;
};

extern PieceColor opponent[];
extern char colorChars[];
extern char essenceChars[];
extern char typeChars[];