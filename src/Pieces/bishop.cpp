#include "pieces.h"

Bishop::Bishop(Essence essence, Owner owner) {
    this->name = "Bishop";
    this->tag = 'B';
    this->essence = essence;
    this->owner = owner;

    switch (essence) {
        case::Classic:
            this->movements.push_back(Movement(AttackMove, -1, -1, -1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, -1, +1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, +1, +1, +1, 0));
            this->movements.push_back(Movement(AttackMove, -1, +1, -1, +1, 0));
            break;
        case::Red:
            break;
        case::Blue:
            // bishop moves
            this->movements.push_back(Movement(AttackMove, -1, -1, -1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, -1, +1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, +1, +1, +1, 0));
            this->movements.push_back(Movement(AttackMove, -1, +1, -1, +1, 0));
            /// king moves
            this->movements.push_back(Movement(AttackMove, +0, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +0, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +0, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +0, +1, +0, +0, 1));
            break;
        case::Yellow:
            // bishop moves
            this->movements.push_back(Movement(AttackMove, -1, -1, -1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, -1, +1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, +1, +1, +1, 0));
            this->movements.push_back(Movement(AttackMove, -1, +1, -1, +1, 0));
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