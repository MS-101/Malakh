#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include "legalMove.h"

class Board {
public:
    Board();

    const static int COLUMNS = 8;
    const static int ROWS = 8;
    Square* squares[ROWS][COLUMNS];

    bool whiteCheck = false;
    bool blackCheck = false;
    std::list<PieceMovement*> checks;
    
    PieceColor curTurn = White;
    std::list<Piece*> whitePieces;
    std::list<Piece*> blackPieces;
    
    Ghost* curGhost = nullptr;

    Essence whitePawnEssence = Classic, whiteKnightEssence = Classic, whiteBishopEssence = Classic, whiteRookEssence = Classic;
    Essence blackPawnEssence = Classic, blackKnightEssence = Classic, blackBishopEssence = Classic, blackRookEssence = Classic;

    

    void InitBoard(
        Essence whitePawnEssence, Essence whiteRookEssence,
        Essence whiteKnightEssence, Essence whiteBishopEssence,
        Essence blackPawnEssence, Essence blackRookEssence,
        Essence blackKnightEssence, Essence blackBishopEssence
    );
    
    void PrintBoard();
    void PrintMoves();
    void PerformMove(int x1, int y1, int x2, int y2, PieceType promotionType);
    std::list<LegalMove*> GetLegalMoves(PieceColor color);
    std::list<LegalMove*> GetLegalMoves(Piece* curPiece);
private:
    void SetGhost(Ghost* newGhost);
    PieceMovement* GetPin(Piece* curPiece);
    bool AddPiece(Piece* newPiece, int x, int y);
    void ChangePiece(Piece* piece, PieceType type, Essence essence);
    void RemovePiece(Piece* removedPiece);
    void MovePiece(Piece* curPiece, int x, int y);
    void CalculateMoves();
    void CalculateMoves(Piece* curPiece);
    void CalculateMoves(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin);
    Movement* CalculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin);
    Movement* CreateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove);
    void ValidateMoves(PieceColor owner);
    void ValidateMoves(Piece* curPiece, PieceMovement* pin);
    void ValidateMove(Piece* curPiece, Movement* curMovement, PieceMovement* pin);
    bool GetValidity(Piece* curPiece, Movement* curMovement, PieceMovement* pin);
    void RemoveMoves(Piece* curPiece);
    void CutMovement(PieceMovement* curPieceMovement);
    void CutMovement(Piece* curPiece, Movement* curMovement);
    void PrintMoves(Piece* curPiece);
};

#endif