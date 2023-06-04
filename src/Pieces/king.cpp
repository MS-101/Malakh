#include "pieces.h"

King::King(Owner owner) : Piece("King", '#', owner, Classic) {
    this->mobilities.push_back(new Mobility(AttackMove, -1, -1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, +0, -1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, +1, -1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, +1, +0, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, +1, +1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, +0, +1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, -1, +1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, -1, +0, +0, +0, 1));
}