#include "pieces.h"

King::King(Owner owner) : Piece("King", '#', owner, Classic) {
    /*
    // king moves
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, +0, +0, 1));
    this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, +0, +0, 1));
    // castling
    this->mobilities.push_back(new Mobility(Move, QueenSideCastleFlags(), +1, 0, -1, 0, 3));
    this->mobilities.push_back(new Mobility(Move, KingSideCastleFlags(), +1, 0, +1, 0, 2));
    */

    this->mobilities.push_back(new Mobility(Move, MobilityFlags(true, false, false, false, false, false, 0, 0), -1, 0, -1, 0, 3));
}