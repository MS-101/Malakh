#include "pieces.h"

Queen::Queen(Owner owner) : Piece("Queen", 'Q', owner, Classic) {
    // Rook moves
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, -1, +0, 0));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
    // bishop moves
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, -1, -1, 0));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +1, -1, 0));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +1, +1, 0));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, -1, +1, 0));
}