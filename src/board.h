#pragma once

#include "bitboard.h"
#include "piece.h"
#include "mobility.h"
#include "evaluation.h"
#include "hashing.h"
#include <vector>

enum GameResult { Unresolved, WhiteWin, BlackWin, Stalemate};

struct EssenceArgs {
    PieceEssence whitePawn = Classic;
    PieceEssence whiteRook = Classic;
    PieceEssence whiteKnight = Classic;
    PieceEssence whiteBishop = Classic;
    PieceEssence blackPawn = Classic;
    PieceEssence blackRook = Classic;
    PieceEssence blackKnight = Classic;
    PieceEssence blackBishop = Classic;
};

class Board {
public:
    PieceEssence essenceConfig[2*6] = {};
    BitBoard pieces[2*6];
    BitBoard colors[2];
    BitBoard attacks[2];
	BitBoard allPieces, notMoved;
    Ghost ghost;
    int pieceCounts[2*6] = {};
    std::vector<LegalMove> moves[2];
    bool movesValidated = false;
    PieceColor curTurn = White;
    EvalArgs eval{};
    BoardHash hash{};
    
	void initBoard(EssenceArgs essenceArgs);
    void printBoard();
    void printMoves();
    int evalBoard(PieceColor color);
    bool isQuiet();
    bool makeMove(char x1, char y1, char x2, char y2);
    bool makeMove(char x1, char y1, char x2, char y2, PieceType promotion);
    bool makeMove(LegalMove move);
    std::pair<bool, Piece> getPiece(char x, char y);
    std::vector<LegalMove> getLegalMoves();
    std::string toString();
    GameResult getResult();
private:
    void setEssenceConfig(EssenceArgs essenceArgs);
    void clearBoard();
    void addPiece(PieceColor color, PieceType type, char x, char y, bool isNew);
    std::pair<bool, Piece> removePiece(char x, char y);
	void refreshAggregations();
};

class MoveGenerator {
public:
    static void clearMoves(Board* board);
    static void generateMoves(Board* board);
    static void generateMoves(Board* board, Piece piece, char x, char y);
};
