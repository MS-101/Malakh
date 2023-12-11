#include "piece.h"

Piece::Piece(PieceType type, PieceColor color, Essence essence)
{
    this->type = type;
    this->color = color;
    this->essence = essence;
}

Piece::~Piece()
{
    for (auto move : availableMoves)
        delete move;
    availableMoves.clear();
}

std::unordered_map<PieceType, std::unordered_map<Essence, std::list<Mobility*>>> Piece::mobilities = {
    {Pawn, {
        {Classic, {
            new Mobility(Move, MobilityFlags(), +0, +1, +0, +0, 1),
            new Mobility(Attack, MobilityFlags().setVigilant(), -1, +1, +0, +0, 1),
            new Mobility(Attack, MobilityFlags().setVigilant(), +1, +1, +0, +0, 1),
            new Mobility(Move, MobilityFlags().setUninterruptible().setInitiative().setHasty(), +0, +1, +0, +1, 2)
        }},
        {Red, {
            new Mobility(Move, MobilityFlags(), +0, +1, +0, +1, 2),
            new Mobility(AttackMove, MobilityFlags().setVigilant(), -1, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags().setVigilant(), +1, +1, +0, +0, 1)
        }},
        {Blue, {
            // pawn moves
            new Mobility(Move, MobilityFlags(), +0, +1, +0, +0, 1),
            new Mobility(Attack, MobilityFlags().setVigilant(), -1, +1, +0, +0, 1),
            new Mobility(Attack, MobilityFlags().setVigilant(), +1, +1, +0, +0, 1),
            new Mobility(Move, MobilityFlags().setUninterruptible().setInitiative().setHasty(), +0, +1, +0, +1, 2),
            // backward pawn moves
            new Mobility(Move, MobilityFlags(), +0, -1, +0, +0, 1),
            new Mobility(Attack, MobilityFlags(), -1, -1, +0, +0, 1),
            new Mobility(Attack, MobilityFlags(), +1, -1, +0, +0, 1)
        }}
    }},
    {Knight, {
        {Classic, {
            new Mobility(AttackMove, MobilityFlags(), -2, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, -2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, -2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +2, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +2, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -2, +1, +0, +0, 1)
        }},
        {Red, {
            // knight hops
            new Mobility(AttackMove, MobilityFlags(), -2, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, -2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, -2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +2, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +2, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -2, +1, +0, +0, 1),
            // camel hops
            new Mobility(AttackMove, MobilityFlags(), -3, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, -3, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, -3, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +3, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +3, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +3, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +3, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -3, +1, +0, +0, 1)
        }},
        {Blue, {
            // knight hops
            new Mobility(AttackMove, MobilityFlags(), -2, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, -2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, -2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +2, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +2, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -2, +1, +0, +0, 1),
            // king moves
            new Mobility(AttackMove, MobilityFlags(), -1, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +0, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +0, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +0, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +0, +0, +0, 1)
        }}
    }},
    {Bishop, {
        {Classic, {
            new Mobility(AttackMove, MobilityFlags(), -1, -1, -1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, -1, +1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +1, +1, +1, 0),
            new Mobility(AttackMove, MobilityFlags(), -1, +1, -1, +1, 0)
        }},
        {Red, {
            // bishop moves
            new Mobility(AttackMove, MobilityFlags(), -1, -1, -1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, -1, +1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +1, +1, +1, 0),
            new Mobility(AttackMove, MobilityFlags(), -1, +1, -1, +1, 0),
            // forward knight hops
            new Mobility(AttackMove, MobilityFlags(), +2, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -2, +1, +0, +0, 1)
        }},
        {Blue, {
            // bishop moves
            new Mobility(AttackMove, MobilityFlags(), -1, -1, -1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, -1, +1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +1, +1, +1, 0),
            new Mobility(AttackMove, MobilityFlags(), -1, +1, -1, +1, 0),
            /// king moves
            new Mobility(AttackMove, MobilityFlags(), +0, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +0, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +0, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +0, +1, +0, +0, 1)
        }}
    }},
    {Rook, {
        {Classic, {
            new Mobility(AttackMove, MobilityFlags(), -1, +0, -1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, -1, +0, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +0, +1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, +1, +0, +1, 0)
        }},
        {Red, {
            // Rook moves
            new Mobility(AttackMove, MobilityFlags(), -1, +0, -1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, -1, +0, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +0, +1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, +1, +0, +1, 0),
            // forward knight hops
            new Mobility(AttackMove, MobilityFlags(), +2, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +2, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -2, +1, +0, +0, 1),
        }},
        {Blue, {
            // Rook moves
            new Mobility(AttackMove, MobilityFlags(), -1, +0, -1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, -1, +0, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +0, +1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, +1, +0, +1, 0),
            // King moves
            new Mobility(AttackMove, MobilityFlags(), -1, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +1, +0, +0, 1)
        }}
    }},
    {Queen, {
        {Classic, {
            // Rook moves
            new Mobility(AttackMove, MobilityFlags(), -1, +0, -1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, -1, +0, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +0, +1, +0, 0),
            new Mobility(AttackMove, MobilityFlags(), +0, +1, +0, +1, 0),
            // bishop moves
            new Mobility(AttackMove, MobilityFlags(), -1, -1, -1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, -1, +1, -1, 0),
            new Mobility(AttackMove, MobilityFlags(), +1, +1, +1, +1, 0),
            new Mobility(AttackMove, MobilityFlags(), -1, +1, -1, +1, 0)
        }}
    }},
    {King, {
        {Classic, {
            new Mobility(AttackMove, MobilityFlags(), -1, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +0, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, -1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +0, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +1, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), +0, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +1, +0, +0, 1),
            new Mobility(AttackMove, MobilityFlags(), -1, +0, +0, +0, 1),
            // castling
            new Mobility(Move, MobilityFlags().setUninterruptible().setInitiative().setCowardly().setInspiring(-4, 0), -1, 0, -1, 0, 3),
            new Mobility(Move, MobilityFlags().setUninterruptible().setInitiative().setCowardly().setInspiring(3, 0), +1, 0, +1, 0, 2)
        }}
    }}
};

std::list<Mobility*> Piece::getMobilities()
{
    return mobilities[type][essence];
}

void Piece::change(PieceType type, Essence essence)
{
    this->type = type;
    this->essence = essence;
}

Ghost::Ghost(int x, int y, Piece* parent) {
    this->x = x;
    this->y = y;
    this->parent = parent;
}
