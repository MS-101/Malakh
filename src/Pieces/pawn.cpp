#include "pieces.h"

Pawn::Pawn(Owner owner, Essence essence) : Piece("Pawn", 'P', owner, essence) {
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
    }
}