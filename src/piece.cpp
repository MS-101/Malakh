#include "piece.h"

PieceColor opponent[] = { Black, White };
char colorChars[] = { 'W', 'B' };
char essenceChars[] = { 'C', 'R', 'B' };
char typeChars[] = { 'P', 'R', 'N', 'B', 'Q', 'K' };

char getPieceIndex(char color, char type)
{
    return color * 6 + type;
}

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

char Piece::toChar()
{
    switch (type) {
    case Pawn:
        switch (essence) {
        case Classic:
            return 'p';
            break;
        case Red:
            return 'w';
            break;
        case Blue:
            return 'e';
            break;
        }
        break;
    case Rook:
        switch (essence) {
        case Classic:
            return 'r';
            break;
        case Red:
            return 't';
            break;
        case Blue:
            return 'z';
            break;
        }
        break;
    case Knight:
        switch (essence) {
        case Classic:
            return 'n';
            break;
        case Red:
            return 'u';
            break;
        case Blue:
            return 'i';
            break;
        }
        break;
    case Bishop:
        switch (essence) {
        case Classic:
            return 'b';
            break;
        case Red:
            return 'o';
            break;
        case Blue:
            return 'a';
            break;
        }
        break;
    case Queen:
        return 'q';
        break;
    case King:
        return 'k';
        break;
    }
}
