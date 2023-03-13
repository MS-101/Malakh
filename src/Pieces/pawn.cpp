#include "pieces.h"

Pawn::Pawn(Essence essence, Owner owner)  {
    this->name = "Pawn";
    this->tag = 'P';
    this->essence = essence;
    this->owner = owner;

    // TO DO: EN PASSANT
    switch (essence) {
        case::Classic:
            this->movements.push_back(Movement(Move,    +0, +1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  -1, +1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  +1, +1, +0, +0, 1));
            break;
        case::Red:
            this->movements.push_back(Movement(Move,        +0, +1, +0, +1, 2));
            this->movements.push_back(Movement(AttackMove,  -1, +1, +0, +0, 1));
            this->movements.push_back(Movement(AttackMove,  +1, +1, +0, +0, 1));
            break;
        case::Blue:
            // Pawn moves
            this->movements.push_back(Movement(Move,    +0, +1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  -1, +1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  +1, +1, +0, +0, 1));
            // Backward pawn moves
            this->movements.push_back(Movement(Move,    +0, -1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  -1, -1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  +1, -1, +0, +0, 1));
            break;
        case::Yellow:
            this->movements.push_back(Movement(Move,    +0, +1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  -1, +1, +0, +0, 1));
            this->movements.push_back(Movement(Attack,  +1, +1, +0, +0, 1));
            this->movements.push_back(Movement(Move,    -1, +0, +0, +0, 1));
            this->movements.push_back(Movement(Move,    -1, -1, +0, +0, 1));
            this->movements.push_back(Movement(Move,    +0, -1, +0, +0, 1));
            this->movements.push_back(Movement(Move,    +1, -1, +0, +0, 1));
            this->movements.push_back(Movement(Move,    +1, +0, +0, +0, 1));
            break;
    }
}