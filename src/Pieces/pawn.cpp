#include "pieces.h"

Pawn::Pawn(Essence essence, Owner owner)  {
    this->name = "Pawn";
    this->tag = 'P';
    this->essence = essence;
    this->owner = owner;

    // TO DO: EN PASSANT
    switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(Move,    +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  +1, +1, +0, +0, 1));
            break;
        case::Red:
            this->mobilities.push_back(new Mobility(Move,        +0, +1, +0, +1, 2));
            this->mobilities.push_back(new Mobility(AttackMove,  -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove,  +1, +1, +0, +0, 1));
            break;
        case::Blue:
            // Pawn moves
            this->mobilities.push_back(new Mobility(Move,    +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  +1, +1, +0, +0, 1));
            // Backward pawn moves
            this->mobilities.push_back(new Mobility(Move,    +0, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  +1, -1, +0, +0, 1));
            break;
        case::Yellow:
            this->mobilities.push_back(new Mobility(Move,    +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack,  +1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move,    -1, +0, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move,    -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move,    +0, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move,    +1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move,    +1, +0, +0, +0, 1));
            break;
    }
}