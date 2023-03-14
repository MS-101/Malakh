#ifndef MOBILITY_H
#define MOBILITY_H

enum MovementType { Move, Attack, AttackMove };

class Mobility {
    public:
        Mobility(MovementType type, int start_x, int start_y, int direction_x, int direction_y, int limit);

        MovementType type;
        int start_x, start_y, direction_x, direction_y, limit;
};

#endif