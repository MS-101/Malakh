#include "movement.h"

Movement::Movement(int x, int y, int moveCounter, bool legal, Mobility* mobility, Movement* next) {
    this->x = x;
    this->y = y;
    this->moveCounter = moveCounter;
    this->legal = legal;
    this->mobility = mobility;
    this->next = next;
}
