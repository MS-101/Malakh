#pragma once

#define NOMINMAX
#include <string>


enum PieceColor { White, Black };
enum PieceType { Pawn, Rook, Knight, Bishop, Queen, King };
enum PieceEssence { Classic, Red, Blue };

struct Piece {
    PieceColor color = White;
    PieceType type = Pawn;
    PieceEssence essence = Classic;
    char toChar();
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

char getPieceIndex(char color, char type);
PieceColor stringToColor(std::string value);
PieceType stringToType(std::string value);
PieceEssence stringToEssence(std::string value);
std::string colorToString(PieceColor value);
std::string typeToString(PieceType value);
std::string essenceToString(PieceEssence value);