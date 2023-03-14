#include "pieces.h"

Knight::Knight(Essence essence, Owner owner) {
    this->name = "Knight";
    this->tag = 'K';
    this->essence = essence;
    this->owner = owner;

    switch (essence) {
        case::Classic:
            this->mobility.push_back(Mobility(AttackMove, -2, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -1, -2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +1, -2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +2, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +2, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +1, +2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -1, +2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -2, +1, +0, +0, 1));
            break;
        case::Red:
            break;
        case::Blue:
            // Knight hops
            this->mobility.push_back(Mobility(AttackMove, -2, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -1, -2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +1, -2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +2, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +2, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +1, +2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -1, +2, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -2, +1, +0, +0, 1));
            // King moves
            this->mobility.push_back(Mobility(AttackMove, -1, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +0, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +1, -1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +1, +0, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +1, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, +0, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -1, +1, +0, +0, 1));
            this->mobility.push_back(Mobility(AttackMove, -1, +0, +0, +0, 1));
            break;
        case::Yellow:
            break;
    } 
}