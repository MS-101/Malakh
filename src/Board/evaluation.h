#ifndef EVALUATION_H
#define EVALUATION_H

#include "piece.h"
#include <array>

class Evaluation {
public:
    static const int mobilityWeight = 1;
    static int startPhase;
    static std::unordered_map<PieceType, int> pieceMatValues;
    static std::unordered_map<PieceType, int> piecePhaseValues;
    static std::unordered_map<PieceType, std::array<std::array<int, 8>, 8>> mg_pcsq;
    static std::unordered_map<PieceType, std::array<std::array<int, 8>, 8>> eg_pcsq;
    static int getY_pcsq(Piece* piece);
};

#endif