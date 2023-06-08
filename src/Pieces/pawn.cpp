#include "pieces.h"

Pawn::Pawn(Owner owner, Essence essence) : Piece("Pawn", 'P', owner, essence) {
    switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(),       +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(),    -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(),    +1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move, EnPassantMoveFlags(),         +0, +1, +0, +1, 2));
            break;
        case::Red:
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(),           +0, +1, +0, +1, 2));
            this->mobilities.push_back(new Mobility(AttackMove, EnPassantCaptureFlags(),    -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, EnPassantCaptureFlags(),    +1, +1, +0, +0, 1));
            break;
        case::Blue:
            // Pawn moves
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(),       +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(),    -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(),    +1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move, EnPassantMoveFlags(),         +0, +1, +0, +1, 2));
            // Backward pawn moves
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(),       +0, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(),    -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(),    +1, -1, +0, +0, 1));
            break;
    }
}