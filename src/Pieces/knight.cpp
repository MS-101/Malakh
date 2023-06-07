#include "pieces.h"

Knight::Knight(Owner owner, Essence essence) : Piece("Knight", 'K', owner, essence) {
    switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(AttackMove, -2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -2, +1, +0, +0, 1));
            break;
        case::Red:
            // Knight hops
            this->mobilities.push_back(new Mobility(AttackMove, -2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -2, +1, +0, +0, 1));
            // camel hops
            this->mobilities.push_back(new Mobility(AttackMove, -3, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, -3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, -3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +3, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +3, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -3, +1, +0, +0, 1));
            break;
        case::Blue:
            // Knight hops
            this->mobilities.push_back(new Mobility(AttackMove, -2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -2, +1, +0, +0, 1));
            // King moves
            this->mobilities.push_back(new Mobility(AttackMove, -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +0, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +0, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, -1, +0, +0, +0, 1));
            break;
    } 
}