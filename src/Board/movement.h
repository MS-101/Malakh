#ifndef MOVEMENT_H
#define MOVEMENT_H

enum MovementType { Move, Attack, AttackMove };

class Movement {
    public:
        Movement(MovementType type, int start_x, int start_y, int direction_x, int direction_y, int limit);

        MovementType type;
        int start_x, start_y, direction_x, direction_y, limit;
};

#endif