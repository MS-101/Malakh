#include "pieces.h"

Knight::Knight(Essence essence, Owner owner) {
    this->name = "Knight";
    this->tag = 'K';
    this->essence = essence;
    this->owner = owner;

    switch (essence) {
        case::Classic:
            this->movements.push_back(Movement(AttackMove, -2, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, -2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, -2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +2, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +2, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -2, +1, +0, +0, 1));
            break;
        case::Red:
            break;
        case::Blue:
            // Knight hops
            this->movements.push_back(Movement(AttackMove, -2, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, -2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, -2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +2, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +2, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +2, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -2, +1, +0, +0, 1));
            // King moves
            this->movements.push_back(Movement(AttackMove, -1, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +0, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +0, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +0, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +0, +0, +0, 1));
            break;
        case::Yellow:
            break;
    } 
}