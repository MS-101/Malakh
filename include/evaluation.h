#pragma once

#include <piece.h>


struct EvalArgs {
    int matEval[2];
    int curPhase = 0;
    int mg_pcsqEval[2];
    int eg_pcsqEval[2];
    int mobCount[2*6];
    int attCount[2];
    int attWeight[2];
};

class Evaluation {
public:
    static const int pieceMatValues[6];
    static const int piecePhaseValues[6];
    static const int startPhase;
    static const int mg_pcsq[6][64];
    static const int eg_pcsq[6][64];
    static const int mg_pieceMobWeights[6];
    static const int eg_pieceMobWeights[6];
    static const int pieceMobPenalties[6];
    static const int mg_pieceTropismWeights[5];
    static const int eg_pieceTropismWeights[5];
    static const int pieceAttWeights[5];
    static const int safetyTable[100];

    static int get_mg_pcsq(PieceColor color, PieceType type, char x, char y);
    static int get_eg_pcsq(PieceColor color, PieceType type, char x, char y);
};