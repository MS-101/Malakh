#include "pieces.h"

Rook::Rook(Owner owner, Essence essence) : Piece("Rook", 'R', owner, essence) {
    switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, -1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
            break;
        case::Red:
            // Rook moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, -1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
            // forward knight hops
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, +1, +0, +0, 1));
            break;
        case::Blue:
            // Rook moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, +1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
            // King moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +0, +0, 1));
            break;
    }
}