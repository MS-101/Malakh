#include "pieces.h"

Pawn::Pawn(Essence essence, Owner owner)  {
    this->name = "Pawn";
    this->tag = 'P';
    this->essence = essence;
    this->owner = owner;

    // TO DO: EN PASSANT
    switch (essence) {
        case::Classic:
            this->mobility.push_back(Mobility(Move,    +0, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  -1, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  +1, +1, +0, +0, 1));
            break;
        case::Red:
            this->mobility.push_back(Mobility(Move,        +0, +1, +0, +1, 2));
            this->mobility.push_back(Mobility(AttackMove,  -1, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove,  +1, +1, +0, +0, 1));
            break;
        case::Blue:
            // Pawn moves
            this->mobility.push_back(Mobility(Move,    +0, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  -1, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  +1, +1, +0, +0, 1));
            // Backward pawn moves
            this->mobility.push_back(Mobility(Move,    +0, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  -1, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  +1, -1, +0, +0, 1));
            break;
        case::Yellow:
            this->mobility.push_back(Mobility(Move,    +0, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  -1, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(Attack,  +1, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(Move,    -1, +0, +0, +0, 1));
            this->mobility.push_back(Mobility(Move,    -1, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(Move,    +0, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(Move,    +1, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(Move,    +1, +0, +0, +0, 1));
            break;
    }
}