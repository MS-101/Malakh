#include "piece.h"

Piece::Piece(std::string name, char tag, Owner owner, Essence essence) {
    this->name = name;
    this->tag = tag;
    this->owner = owner;
    this->essence = essence;
}