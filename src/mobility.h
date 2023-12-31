#pragma once

#include "chess.h"
#include "vector"
#include "unordered_map"

enum MovementType { Move, Attack, AttackMove };

class Mobility {
public:
    Mobility();
    Mobility(MovementType type, int start_x, int start_y, int direction_x, int direction_y, int limit);

    MovementType type = Move;
    int start_x = 0; 
    int start_y = 0;
    int direction_x = 0;
    int direction_y = 0; 
    int limit = 0;
};

class MobilityConfig {
public:
    static std::unordered_map<PieceEssence, std::vector<Mobility>> pawnConfig;
    static std::unordered_map<PieceEssence, std::vector<Mobility>> rookConfig;
    static std::unordered_map<PieceEssence, std::vector<Mobility>> knightConfig;
    static std::unordered_map<PieceEssence, std::vector<Mobility>> bishopConfig;
    static std::unordered_map<PieceEssence, std::vector<Mobility>> queenConfig;
    static std::unordered_map<PieceEssence, std::vector<Mobility>> kingConfig;
};