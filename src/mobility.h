#pragma once

#include "piece.h"
#include <vector>

enum MovementType { Move, Attack, AttackMove };

class Mobility {
public:
    Mobility();
    Mobility(MovementType type, int start_x, int start_y, int direction_x, int direction_y, int limit);

    MovementType type = Move;
    int start_x = 0;
    int start_y = 0;
    int direction_x = 0;
    int direction_y = 0;
    int limit = 0;
};

extern std::vector<Mobility> mobilityConfig[6][3];