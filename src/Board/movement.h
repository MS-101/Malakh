#ifndef MOVE_H
#define MOVE_H

#include "mobility.h"

enum MoveState { Blocked, Available };

class Movement {
    public:
        Movement(MoveState moveState, Mobility* mobility, Movement* next, int x, int y);

        MoveState moveState;
        Mobility* mobility;
        Movement* next;
        int x, y;
};

#endif