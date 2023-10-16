#include "legalMove.h"

LegalMove::LegalMove(PieceMovement* pieceMovement, PieceType promotionType) {
    this->pieceMovement = pieceMovement;
    this->promotionType = promotionType;
}