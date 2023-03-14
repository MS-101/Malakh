#include "pieces.h"

Rook::Rook(Essence essence, Owner owner) {
    this->name = "Rook";
    this->tag = 'R';
    this->essence = essence;
    this->owner = owner;

    // TO DO: CASTLING
    switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(AttackMove, -1, +0, -1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +0, -1, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +0, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +0, +1, +0, +0, 0));
            break;
        case::Red:
            break;
        case::Blue:
            // Rook moves
            this->mobilities.push_back(new Mobility(AttackMove, -1, +0, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +0, -1, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +0, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +0, +1, +0, +0, 0));
            // King moves
            this->mobilities.push_back(new Mobility(AttackMove, -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +1, +0, +0, 1));
            break;
        case::Yellow:
            // Rook moves
            this->mobilities.push_back(new Mobility(AttackMove, -1, +0, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +0, -1, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +0, +0, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, +0, +1, +0, +0, 0));
            // knight hops
            this->mobilities.push_back(new Mobility(AttackMove, -2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -2, +1, +0, +0, 1));
            break;
    }
}