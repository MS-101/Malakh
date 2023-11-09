#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include "legalMove.h"

class Board {
public:
    Board();
    Board(Board* board);
    ~Board();

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
    void printBoard();
    void printMoves();
    void performMove(int x1, int y1, int x2, int y2, PieceType promotionType);
    std::list<LegalMove*> getLegalMoves(PieceColor color);
    std::list<LegalMove*> getLegalMoves(Piece* curPiece);
private:
    void setGhost(Ghost* newGhost);
    PieceMovement* getPin(Piece* curPiece);
    bool addPiece(Piece* newPiece, int x, int y);
    void changePiece(Piece* piece, PieceType type, Essence essence);
    void removePiece(Piece* removedPiece);
    void movePiece(Piece* curPiece, int x, int y);
    void calculateMoves();
    void calculateMoves(Piece* curPiece);
    void calculateMoves(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin);
    Movement* calculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin);
    Movement* createMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove);
    void validateMoves(PieceColor owner);
    void validateMoves(Piece* curPiece, PieceMovement* pin);
    void validateMove(Piece* curPiece, Movement* curMovement, PieceMovement* pin);
    bool getValidity(Piece* curPiece, Movement* curMovement, PieceMovement* pin);
    void removeMoves(Piece* curPiece);
    void cutMovement(PieceMovement* curPieceMovement);
    void cutMovement(Piece* curPiece, Movement* curMovement);
    void printMoves(Piece* curPiece);
};

#endif