#include "piece.h"

Piece::Piece(std::string name, char tag, Owner owner, Essence essence) {
    this->name = name;
    this->tag = tag;
    this->owner = owner;
    this->essence = essence;
}

Ghost::Ghost(int x, int y, Piece* parent) {
    this->x = x;
    this->y = y;
    this->parent = parent;
}