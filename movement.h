#ifndef MOVEMENT_H
#define MOVEMENT_H

enum MovementType { move, attack, attackMove};

class Movement {
    public:
        Movement(MovementType type, int start_x, int start_y, int direction_x, int direction_y, int limit);

        MovementType type;
        int start_x;
        int start_y;
        int direction_x;
        int direction_y;
        int limit;
};

#endif