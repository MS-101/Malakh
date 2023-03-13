#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include "piece.h"

class Board {
    public:
        const static int COLUMNS = 8;
        const static int ROWS = 8;

        Square *squares[COLUMNS][ROWS];

        Board();

        //void InitMoves();
        //void CalculateMoves();
        //void MovePiece(int x1, int y1, int x2, int y2);
        void PrintBoard();
        //void PrintMoves(Piece *curPiece);

    //private:
        //void RemoveMoves(Piece *curPiece);
        //void CalculateMoves(Piece *curPiece);
};

#endif