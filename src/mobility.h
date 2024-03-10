#pragma once

#include "piece.h"
#include <vector>
#include <string>

enum MovementType { Move, Attack, AttackMove };

enum Castling { none, kingSide, queenSide };

struct MobilityFlags {
    bool initiative = false;
    bool hasty = false;
    bool uninterruptible = false;
    bool vigilant = false;
};

struct Mobility {
    MovementType type = Move;
    int start_x = 0;
    int start_y = 0;
    int direction_x = 0;
    int direction_y = 0;
    int limit = 0;
    MobilityFlags flags{};
};

struct LegalMove {
    LegalMove();
    LegalMove(int x1, int y1, int x2, int y2, Mobility mobility);
    LegalMove(std::string value);

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    Mobility mobility;
    PieceType promotion = Pawn;
    Castling castling = none;

    std::string toString();
    std::string toStringWithFlags(PieceColor color);
};

class Mobilities {
public:
    static std::vector<Mobility> mobilityConfig[6][3];
    static void printMobilities(PieceType type, PieceEssence essence);
};
