#include "mobility.h"

Mobility::Mobility(MovementType type, int start_x, int start_y, int direction_x, int direction_y, int limit)
{
    this->type = type;
    this->start_x = start_x;
    this->start_y = start_y;
    this->direction_x = direction_x;
    this->direction_y = direction_y;
    this->limit = limit;
}

std::unordered_map<PieceEssence, std::vector<Mobility>> MobilityConfig::pawnConfig = {
    {Classic, {
        {Move, +0, +1, +0, +0, 1},
        {Attack, -1, +1, +0, +0, 1},
        {Attack, +1, +1, +0, +0, 1}
    }},
    {Red, {
        {Move, +0, +1, +0, +1, 2},
        {AttackMove, -1, +1, +0, +0, 1},
        {AttackMove, +1, +1, +0, +0, 1}
    }},
    {Blue, {
        // pawn moves
        {Move, +0, +1, +0, +0, 1},
        {Attack, -1, +1, +0, +0, 1},
        {Attack, +1, +1, +0, +0, 1},
        // backward pawn moves
        {Move, +0, -1, +0, +0, 1},
        {Attack, -1, -1, +0, +0, 1},
        {Attack, +1, -1, +0, +0, 1}
    }}
};

std::unordered_map<PieceEssence, std::vector<Mobility>> MobilityConfig::rookConfig = {
    {Classic, {
        {AttackMove, -1, +0, -1, +0, 0},
        {AttackMove, +0, -1, +0, -1, 0},
        {AttackMove, +1, +0, +1, +0, 0},
        {AttackMove, +0, +1, +0, +1, 0}
    }},
    {Red, {
        // Rook moves
        {AttackMove, -1, +0, -1, +0, 0},
        {AttackMove, +0, -1, +0, -1, 0},
        {AttackMove, +1, +0, +1, +0, 0},
        {AttackMove, +0, +1, +0, +1, 0},
        // forward knight hops
        {AttackMove, +2, +1, +0, +0, 1},
        {AttackMove, +1, +2, +0, +0, 1},
        {AttackMove, -1, +2, +0, +0, 1},
        {AttackMove, -2, +1, +0, +0, 1},
    }},
    {Blue, {
        // Rook moves
        {AttackMove, -1, +0, -1, +0, 0},
        {AttackMove, +0, -1, +0, -1, 0},
        {AttackMove, +1, +0, +1, +0, 0},
        {AttackMove, +0, +1, +0, +1, 0},
        // King moves
        {AttackMove, -1, -1, +0, +0, 1},
        {AttackMove, -1, +1, +0, +0, 1},
        {AttackMove, +1, -1, +0, +0, 1},
        {AttackMove, +1, +1, +0, +0, 1}
    }}
};

std::unordered_map<PieceEssence, std::vector<Mobility>> MobilityConfig::knightConfig = {
    {Classic, {
        {AttackMove, -2, -1, +0, +0, 1},
        {AttackMove, -1, -2, +0, +0, 1},
        {AttackMove, +1, -2, +0, +0, 1},
        {AttackMove, +2, -1, +0, +0, 1},
        {AttackMove, +2, +1, +0, +0, 1},
        {AttackMove, +1, +2, +0, +0, 1},
        {AttackMove, -1, +2, +0, +0, 1},
        {AttackMove, -2, +1, +0, +0, 1}
    }},
    {Red, {
        // knight hops
        {AttackMove, -2, -1, +0, +0, 1),
        {AttackMove, -1, -2, +0, +0, 1},
        {AttackMove, +1, -2, +0, +0, 1},
        {AttackMove, +2, -1, +0, +0, 1},
        {AttackMove, +2, +1, +0, +0, 1},
        {AttackMove, +1, +2, +0, +0, 1},
        {AttackMove, -1, +2, +0, +0, 1},
        {AttackMove, -2, +1, +0, +0, 1},
        // additional hops
        {AttackMove, -3, +0, +0, +0, 1},
        {AttackMove, +3, +0, +0, +0, 1},
        {AttackMove, +0, -3, +0, +0, 1},
        {AttackMove, +0, +3, +0, +0, 1},
    }},
    {Blue, {
        // knight hops
        {AttackMove, -2, -1, +0, +0, 1},
        {AttackMove, -1, -2, +0, +0, 1},
        {AttackMove, +1, -2, +0, +0, 1},
        {AttackMove, +2, -1, +0, +0, 1},
        {AttackMove, +2, +1, +0, +0, 1},
        {AttackMove, +1, +2, +0, +0, 1},
        {AttackMove, -1, +2, +0, +0, 1},
        {AttackMove, -2, +1, +0, +0, 1},
        // king moves
        {AttackMove, -1, -1, +0, +0, 1},
        {AttackMove, +0, -1, +0, +0, 1},
        {AttackMove, +1, -1, +0, +0, 1},
        {AttackMove, +1, +0, +0, +0, 1},
        {AttackMove, +1, +1, +0, +0, 1},
        {AttackMove, +0, +1, +0, +0, 1},
        {AttackMove, -1, +1, +0, +0, 1},
        {AttackMove, -1, +0, +0, +0, 1}
    }}
};

std::unordered_map<PieceEssence, std::vector<Mobility>> MobilityConfig::bishopConfig = {
    {Classic, {
        {AttackMove, -1, -1, -1, -1, 0},
        {AttackMove, +1, -1, +1, -1, 0},
        {AttackMove, +1, +1, +1, +1, 0},
        {AttackMove, -1, +1, -1, +1, 0}
    }},
    {Red, {
        // bishop moves
        {AttackMove, -1, -1, -1, -1, 0},
        {AttackMove, +1, -1, +1, -1, 0},
        {AttackMove, +1, +1, +1, +1, 0},
        {AttackMove, -1, +1, -1, +1, 0},
        // forward knight hops
        {AttackMove, +2, +1, +0, +0, 1},
        {AttackMove, +1, +2, +0, +0, 1},
        {AttackMove, -1, +2, +0, +0, 1},
        {AttackMove, -2, +1, +0, +0, 1}
    }},
    {Blue, {
        // bishop moves
        {AttackMove, -1, -1, -1, -1, 0},
        {AttackMove, +1, -1, +1, -1, 0},
        {AttackMove, +1, +1, +1, +1, 0},
        {AttackMove, -1, +1, -1, +1, 0},
        /// king moves
        {AttackMove, +0, -1, +0, +0, 1},
        {AttackMove, -1, +0, +0, +0, 1},
        {AttackMove, +1, +0, +0, +0, 1},
        {AttackMove, +0, +1, +0, +0, 1}
    }}
};

std::unordered_map<PieceEssence, std::vector<Mobility>> MobilityConfig::queenConfig = {
    {Classic, {
        // Rook moves
        {AttackMove, -1, +0, -1, +0, 0},
        {AttackMove, +0, -1, +0, -1, 0},
        {AttackMove, +1, +0, +1, +0, 0},
        {AttackMove, +0, +1, +0, +1, 0},
        // bishop moves
        {AttackMove, -1, -1, -1, -1, 0},
        {AttackMove, +1, -1, +1, -1, 0},
        {AttackMove, +1, +1, +1, +1, 0},
        {AttackMove, -1, +1, -1, +1, 0}
    }}
};

std::unordered_map<PieceEssence, std::vector<Mobility>> MobilityConfig::kingConfig = {
    {Classic, {
        {AttackMove, -1, -1, +0, +0, 1},
        {AttackMove, +0, -1, +0, +0, 1},
        {AttackMove, +1, -1, +0, +0, 1},
        {AttackMove, +1, +0, +0, +0, 1},
        {AttackMove, +1, +1, +0, +0, 1},
        {AttackMove, +0, +1, +0, +0, 1},
        {AttackMove, -1, +1, +0, +0, 1},
        {AttackMove, -1, +0, +0, +0, 1},
    }}
};
