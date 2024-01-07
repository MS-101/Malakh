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
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    Mobility mobility;
    PieceType promotion = Pawn;
    Castling castling = none;

    std::string toString();
};

extern std::vector<Mobility> mobilityConfig[6][3];