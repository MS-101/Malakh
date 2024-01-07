#pragma once

#include "piece.h"
#include <vector>

enum MovementType { Move, Attack, AttackMove };

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

extern std::vector<Mobility> mobilityConfig[6][3];