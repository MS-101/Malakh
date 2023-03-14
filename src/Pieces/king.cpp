#include "pieces.h"

King::King(Owner owner) {
    this->name = "King";
    this->tag = '#';
    this->essence = Classic;
    this->owner = owner;

    // TO DO: KING SAFETY
    this->mobility.push_back(Mobility(AttackMove, -1, -1, +0, +0, 1));
    this->mobility.push_back(Mobility(AttackMove, +0, -1, +0, +0, 1));
    this->mobility.push_back(Mobility(AttackMove, +1, -1, +0, +0, 1));
    this->mobility.push_back(Mobility(AttackMove, +1, +0, +0, +0, 1));
    this->mobility.push_back(Mobility(AttackMove, +1, +1, +0, +0, 1));
    this->mobility.push_back(Mobility(AttackMove, +0, +1, +0, +0, 1));
    this->mobility.push_back(Mobility(AttackMove, -1, +1, +0, +0, 1));
    this->mobility.push_back(Mobility(AttackMove, -1, +0, +0, +0, 1));
}