#ifndef MOVE_H
#define MOVE_H

#include "mobility.h"

class Movement {
    public:
        Movement(int x, int y, int moveCounter, bool legal, Mobility* mobility, Movement* next);

        int x, y, moveCounter;
        bool legal;
        Mobility* mobility;
        Movement* next;
};

#endif