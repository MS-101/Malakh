#include "pieces.h"

Queen::Queen(Owner owner) {
    this->name = "Queen";
    this->tag = 'Q';
    this->essence = Classic;
    this->owner = owner;

    // Rook moves
    this->mobilities.push_back(new Mobility(AttackMove, -1, +0, +0, +0, 0));
    this->mobilities.push_back(new Mobility(AttackMove, +0, -1, +0, +0, 0));
    this->mobilities.push_back(new Mobility(AttackMove, +1, +0, +0, +0, 0));
    this->mobilities.push_back(new Mobility(AttackMove, +0, +1, +0, +0, 0));
    // bishop moves
    this->mobilities.push_back(new Mobility(AttackMove, -1, -1, -1, -1, 0));
    this->mobilities.push_back(new Mobility(AttackMove, +1, -1, +1, -1, 0));
    this->mobilities.push_back(new Mobility(AttackMove, +1, +1, +1, +1, 0));
    this->mobilities.push_back(new Mobility(AttackMove, -1, +1, -1, +1, 0));
}