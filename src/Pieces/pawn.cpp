#include "pieces.h"

Pawn::Pawn(Essence essence, Owner owner)  {
    this->name = "Pawn";
    this->tag = 'P';
    this->essence = essence;
    this->owner = owner;
}