#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include "list"

class Board {
    public:
        Board();

        const static int COLUMNS = 8;
        const static int ROWS = 8;

        Square *squares[ROWS][COLUMNS];
        std::list<Piece*> whitePieces;
        std::list<Piece*> blackPieces;

        void InitBoard(
            Essence whitePawnEssence, Essence whiteRookEssence, 
            Essence whiteKnightEssence, Essence whiteBishopEssence,
            Essence blackPawnEssence, Essence blackRookEssence, 
            Essence blackKnightEssence, Essence blackBishopEssence
        );
        void MovePiece(int x1, int y1, int x2, int y2);
        void PrintBoard();
        void PrintMoves(Piece* curPiece);
    private:
        bool AddPiece(Piece* newPiece, int x, int y);
        void InitMoves();
        void CalculateMoves(Piece* curPiece);
        Movement* CalculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, int* cur_x, int* cur_y);
};

#endif