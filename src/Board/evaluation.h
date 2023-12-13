#ifndef EVALUATION_H
#define EVALUATION_H

#include "piece.h"
#include <array>

class Evaluation {
public:
    static const int startPhase;
    static const int safetyTable[100];
    static std::unordered_map<PieceType, int> pieceMatValues;
    static std::unordered_map<PieceType, int> piecePhaseValues;
    static std::unordered_map<PieceType, int> pieceAttWeights;
    static std::unordered_map<PieceType, int> pieceMobPenalties;
    static std::unordered_map<PieceType, int> mg_pieceTropismWeights;
    static std::unordered_map<PieceType, int> eg_pieceTropismWeights;
    static std::unordered_map<PieceType, int> mg_pieceMobWeights;
    static std::unordered_map<PieceType, int> eg_pieceMobWeights;
    static std::unordered_map<PieceType, std::array<std::array<int, 8>, 8>> mg_pcsq;
    static std::unordered_map<PieceType, std::array<std::array<int, 8>, 8>> eg_pcsq;
    static int getY_pcsq(Piece* piece);
};

#endif