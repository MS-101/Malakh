#pragma once

#include "chess.h"
#include "bitboard.h"
#include "mobility.h"
#include <list>

struct EssenceConfig {
    PieceEssence whitePawn = Classic;
    PieceEssence whiteRook = Classic;
    PieceEssence whiteKnight = Classic;
    PieceEssence whiteBishop = Classic;
    PieceEssence blackPawn = Classic;
    PieceEssence blackRook = Classic;
    PieceEssence blackKnight = Classic;
    PieceEssence blackBishop = Classic;
};

struct LegalMove {
    int x1, y1, x2, y2;
    Mobility mobility;
};

class Board {    
public:
    EssenceConfig essenceConfig;
	BitBoard whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueens, whiteKings;
	BitBoard blackPawns, blackRooks, blackKnights, blackBishops, blackQueens, blackKings;
	BitBoard whitePieces, blackPieces, allPieces, notMoved;
	BitBoard whiteAttacks, blackAttacks;
    std::list<LegalMove> whiteMoves, blackMoves;
	void initBoard(EssenceConfig essenceConfig);
    void makeMove(char x1, char y1, char x2, char y2);
    std::list<LegalMove> getLegalMoves(PieceColor color);
private:
	void refreshAggregations();  
};

class MoveGenerator {
public:
    static void generateMoves(Board board);
};
