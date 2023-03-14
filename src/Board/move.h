#ifndef MOVE_H
#define MOVE_H

#include "mobility.h"

enum MoveState { Blocked, Available };

class Move {
    public:
        Move();

        MoveState moveState;
        Mobility *mobility;
        Move *next;
};

#endif