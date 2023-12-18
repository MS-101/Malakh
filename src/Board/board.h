#ifndef BOARD_H
#define BOARD_H

#include "transpositions.h"
#include "evaluation.h"
#include "square.h"
#include <vector>
#include <stack>

struct MoveRecord {
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
    int ghostX = -1, ghostY = -1, ghostParentX = -1, ghostParentY = -1;
    int inspiredX1 = -1, inspiredY1 = -1, inspiredX2 = -1, inspiredY2 = -1;
    bool performedCapture = false, capturedHasMoved = false, performedPromotion = false, performedInspiration = false, hadGhost = false, hasMoved = false;
    PieceType capturedType = Pawn;
};

struct PinArgs {
    bool kingHit = false;
    PieceMovement* pin = nullptr;
    Piece* pinnedPiece = nullptr;
    int pinnedCount = 0;
};

class Board {
public:
    Board();
    Board(Essence whitePawnEssence, Essence whiteRookEssence, Essence whiteKnightEssence, Essence whiteBishopEssence, Essence blackPawnEssence, Essence blackRookEssence, Essence blackKnightEssence, Essence blackBishopEssence);
    Board(Board* board);
    ~Board();

    const static int COLUMNS = 8;
    const static int ROWS = 8;
    Square* squares[ROWS][COLUMNS];

    bool whiteCheck = false;
    bool blackCheck = false;
    std::list<PieceMovement*> checks;
    
    PieceColor curTurn = White;
    Piece* whiteKing = nullptr;
    Piece* blackKing = nullptr;
    std::list<Piece*> whitePieces;
    std::list<Piece*> blackPieces;
    
    Ghost* curGhost = nullptr;

    Essence whitePawnEssence = Classic, whiteKnightEssence = Classic, whiteBishopEssence = Classic, whiteRookEssence = Classic;
    Essence blackPawnEssence = Classic, blackKnightEssence = Classic, blackBishopEssence = Classic, blackRookEssence = Classic;

    std::stack<struct MoveRecord> moveHistory;

    unsigned long long hash = 0;
    int curPhase = 0;
    std::unordered_map<PieceColor, int> matEval;
    std::unordered_map<PieceColor, int> mg_pcsqEval;
    std::unordered_map<PieceColor, int> eg_pcsqEval;

    void initBoard(Essence whitePawnEssence, Essence whiteRookEssence, Essence whiteKnightEssence, Essence whiteBishopEssence,
        Essence blackPawnEssence, Essence blackRookEssence, Essence blackKnightEssence, Essence blackBishopEssence);
    void makeMove(int x1, int y1, int x2, int y2);
    void makeMove(int x1, int y1, int x2, int y2, PieceType promotionType);
    void unmakeMove();
    std::vector<LegalMove> getLegalMoves(PieceColor color);
    void printBoard();
    void printMoves();
    void printVirtualMoves();
private:
    bool initialized = false;
    void setGhost(Ghost* newGhost);
    PieceMovement* getPin(Piece* curPiece);
    bool addPiece(Piece* newPiece, int x, int y);
    Piece* copyPiece(Piece* piece);
    void changePiece(Piece* piece, PieceType type, Essence essence);
    void removePiece(Piece* removedPiece);
    void movePiece(Piece* curPiece, int x, int y, bool hasMoved);
    void calculateMoves();
    void calculateMoves(Piece* curPiece);
    void calculateMoves(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin);
    Movement* calculateMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PieceMovement* pin);
    void calculateVirtualMoves(Piece* curPiece, Mobility* curMobility);
    Movement* calculateVirtualMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove, PinArgs* pinArgs);
    Movement* createMove(Piece* curPiece, Mobility* curMobility, Movement* prevMove);
    void validateMoves(PieceColor owner);
    void validateMoves(Piece* curPiece, PieceMovement* pin);
    void validateMove(Piece* curPiece, Movement* curMovement, PieceMovement* pin);
    bool getValidity(Piece* curPiece, Movement* curMovement, PieceMovement* pin);
    void removeMoves(Piece* curPiece);
    void removeVirtualMoves(Piece* curPiece, Movement* curMovement);
    void calculateInspiringMoves(Piece* inspiredPiece);
    void removeInspiringMoves(Piece* inspiredPiece);
    void cutMovement(PieceMovement* curPieceMovement);
    void cutMovement(Piece* curPiece, Movement* curMovement);
    std::list<LegalMove> getLegalMoves(Piece* curPiece);
    void printMoves(Piece* curPiece);
    void printVirtualMoves(Piece* curPiece);
    void makePins(Square* curSquare);
    void removePins(Square* curSquare);
    void unblockPins(Square* curSquare);
    void blockPins(Square* curSquare);
};

#endif