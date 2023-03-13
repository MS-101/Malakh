#include "pieces.h"

Queen::Queen(Essence essence, Owner owner) {
    this->name = "Queen";
    this->tag = 'Q';
    this->essence = essence;
    this->owner = owner;

    switch (essence) {
        case::Classic:
            // Rook moves
            this->movements.push_back(Movement(AttackMove, -1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, -1, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +1, +0, +0, +0, 0));
            this->movements.push_back(Movement(AttackMove, +0, +1, +0, +0, 0));
            // bishop moves
            this->movements.push_back(Movement(AttackMove, -1, -1, -1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, -1, +1, -1, 0));
            this->movements.push_back(Movement(AttackMove, +1, +1, +1, +1, 0));
            this->movements.push_back(Movement(AttackMove, -1, +1, -1, +1, 0));
            break;
        case::Red:
            // Queen is already powerful enough.
            break;
        case::Blue:
            // Queen is already powerful enough.
            break;
        case::Yellow:
            // Queen is already powerful enough.
            break;
    }
}