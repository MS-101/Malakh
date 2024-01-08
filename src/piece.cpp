#include "piece.h"

PieceColor opponent[] = { Black, White };
char colorChars[] = { 'W', 'B' };
char essenceChars[] = { 'C', 'R', 'B' };
char typeChars[] = { 'P', 'R', 'N', 'B', 'Q', 'K' };

PieceColor stringToColor(std::string value)
{
    if (value == "Black")
        return Black;
    else
        return White;
}

PieceType stringToType(std::string value)
{
    if (value == "Rook")
        return Rook;
    else if (value == "Knight")
        return Knight;
    else if (value == "Bishop")
        return Bishop;
    else if (value == "Queen")
        return Queen;
    else if (value == "King")
        return King;
    else
        return Pawn;
}

PieceEssence stringToEssence(std::string value)
{
    if (value == "Red")
        return Red;
    else if (value == "Blue")
        return Blue;
    else
        return Classic;
}

std::string colorToString(PieceColor value)
{
    switch (value) {
    case Black:
        return "Black";
        break;
    default:
        return "White";
        break;
    }
}

std::string typeToString(PieceType value)
{
    switch (value) {
    case Rook:
        return "Rook";
        break;
    case Knight:
        return "Knight";
        break;
    case Bishop:
        return "Bishop";
        break;
    case Queen:
        return "Queen";
        break;
    case King:
        return "King";
        break;
    default:
        return "Pawn";
        break;
    }
}

std::string essenceToString(PieceEssence value)
{
    switch (value) {
    case Red:
        return "Red";
        break;
    case Blue:
        return "Blue";
        break;
    default:
        return "Classic";
        break;
    }
}
