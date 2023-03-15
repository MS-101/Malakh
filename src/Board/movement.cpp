#include "movement.h"

Movement::Movement(int x, int y, bool legal, Mobility* mobility, Movement* next) {
    this->x = x;
    this->y = y;
    this->legal = legal;
    this->mobility = mobility;
    this->next = next;
}