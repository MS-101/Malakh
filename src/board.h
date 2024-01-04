#pragma once

#include "bitboard.h"
#include "piece.h"
#include "mobility.h"
#include "evaluation.h"
#include <vector>
#include <string>

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

enum Castling { none, kingSide, queenSide };

struct LegalMove {
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    Mobility mobility;
    Castling castling = none;

    std::string toString();
};

class Board {
public:
    PieceEssence essenceConfig[2][6] = {};
    BitBoard pieces[2][6];
    BitBoard colors[2];
    BitBoard attacks[2];
	BitBoard allPieces, notMoved;
    int pieceCounts[2][6] = {};
    std::vector<LegalMove> moves[2];
    PieceColor curTurn = White;
    EvalArgs eval{};
    
	void initBoard(EssenceArgs essenceArgs);
    void printBoard();
    void printMoves();
    int evalBoard(PieceColor maximizingPlayer);
    bool isQuiet();
    bool makeMove(LegalMove move);
    std::pair<bool, Piece> getPiece(char x, char y);
    std::vector<LegalMove> getLegalMoves();
private:
    void setEssenceConfig(EssenceArgs essenceArgs);
    void clearBoard();
    void addPiece(PieceColor color, PieceType type, char x, char y);
    std::pair<bool, Piece> removePiece(char x, char y);
	void refreshAggregations();
};

class MoveGenerator {
public:
    static void clearMoves(Board* board);
    static void generateMoves(Board* board);
    static void generateMoves(Board* board, Piece piece, char x, char y);
};
