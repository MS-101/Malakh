#include "pieces.h"

King::King(Essence essence, Owner owner) {
    this->name = "King";
    this->tag = '#';
    this->essence = essence;
    this->owner = owner;

    // TO DO: KING SAFETY
    switch (essence) {
        case::Classic:
            this->movements.push_back(Movement(AttackMove, -1, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +0, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, -1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +0, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +1, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, +0, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove, -1, +0, +0, +0, 1));
            break;
        case::Red:
            // Game would be impossible to win if I implemented this.
            break;
        case::Blue:
            // Game would be impossible to win if I implemented this.
            break;
        case::Yellow:
            // Game would be impossible to win if I implemented this.
            break;
    }
}