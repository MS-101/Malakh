#ifndef BOARD_H
#define BOARD_H

#include "square.h"

class Board {
    public:
        Board();

        const static int COLUMNS = 8;
        const static int ROWS = 8;

        Square *squares[ROWS][COLUMNS];

        void PrintBoard();
};

#endif