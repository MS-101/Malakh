#include "movement.h"

Movement::Movement(MoveState moveState, Mobility* mobility, Movement* next, int x, int y) {
    this->moveState = moveState;
    this->mobility = mobility;
    this->next = next;
    this->x = x;
    this->y = y;
}