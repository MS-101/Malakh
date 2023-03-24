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
        void PrintBoard();
        void PrintMoves();
        void PerformMove(int x1, int y1, int x2, int y2);
    private:
        bool AddPiece(Piece* newPiece, int x, int y);
        void RemovePiece(Piece* removedPiece);
        void MovePiece(Piece* curPiece, int x, int y);
        void CalculateMoves();
        void CalculateMoves(Piece* curPiece);
        void CalculateMoves(Piece* curPiece, Mobility* curMobility, Movement* prevMove);
        Movement* CalculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove);
        void RemoveMoves(Piece* curPiece);
        void CutMovement(PieceMovement* curMovement);
        void PrintMoves(Piece* curPiece);
};

#endif