#include "pieces.h"

Rook::Rook(Essence essence, Owner owner) {
    this->name = "Rook";
    this->tag = 'R';
    this->essence = essence;
    this->owner = owner;

    // TO DO: CASTLING
    switch (essence) {
        case::Classic:
            this->movements.push_back(Movement(AttackMove, -1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, -1, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, +1, +0, +0, 0));
            break;
        case::Red:
            break;
        case::Blue:
            // Rook moves
            this->movements.push_back(Movement(AttackMove, -1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, -1, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, +1, +0, +0, 0));
            // King moves
            this->movements.push_back(Movement(AttackMove, -1, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +1, +0, +0, 1));
            break;
        case::Yellow:
            // Rook moves
            this->movements.push_back(Movement(AttackMove, -1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, -1, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, +1, +0, +0, 0));
            // knight hops
            this->movements.push_back(Movement(AttackMove, -2, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, -2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, -2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +2, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +2, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -2, +1, +0, +0, 1));
            break;
    }
}