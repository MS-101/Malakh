#include "piece.h"

Piece::Piece(PieceType type, PieceColor color, Essence essence)
{
    this->color = color;
    setMobilities(type, essence);
}

Piece::~Piece()
{
    for (auto mobilitiesIterator = mobilities.begin(); mobilitiesIterator != mobilities.end(); ++mobilitiesIterator)
        delete* mobilitiesIterator;
    mobilities.clear();

    for (auto moveIterator = availableMoves.begin(); moveIterator != availableMoves.end(); ++moveIterator)
        delete* moveIterator;
    availableMoves.clear();
}

void Piece::setMobilities(PieceType type, Essence essence)
{
    this->type = type;
    this->essence = essence;

    this->mobilities.clear();

    switch (type) {
    case::Pawn:
        switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(), +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(), -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(), +1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move, EnPassantMoveFlags(), +0, +1, +0, +1, 2));
            break;
        case::Red:
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(), +0, +1, +0, +1, 2));
            this->mobilities.push_back(new Mobility(AttackMove, EnPassantCaptureFlags(), -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, EnPassantCaptureFlags(), +1, +1, +0, +0, 1));
            break;
        case::Blue:
            // Pawn moves
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(), +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(), -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(), +1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Move, EnPassantMoveFlags(), +0, +1, +0, +1, 2));
            // Backward pawn moves
            this->mobilities.push_back(new Mobility(Move, DefaultMobilityFlags(), +0, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(), -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(Attack, EnPassantCaptureFlags(), +1, -1, +0, +0, 1));
            break;
        }
        break;
    case::Knight:
        switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, +1, +0, +0, 1));
            break;
        case::Red:
            // Knight hops
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, +1, +0, +0, 1));
            // camel hops
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -3, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +3, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +3, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +3, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -3, +1, +0, +0, 1));
            break;
        case::Blue:
            // Knight hops
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, +1, +0, +0, 1));
            // King moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, +0, +0, 1));
            break;
        }
        break;
    case::Bishop:
        switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, -1, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +1, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +1, +1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, -1, +1, 0));
            break;
        case::Red:
            // bishop moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, -1, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +1, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +1, +1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, -1, +1, 0));
            // forward knight hops
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, +1, +0, +0, 1));
            break;
        case::Blue:
            // bishop moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, -1, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +1, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +1, +1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, -1, +1, 0));
            /// king moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +0, 1));
            break;
        }
        break;
    case::Rook:
        switch (essence) {
        case::Classic:
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, -1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
            break;
        case::Red:
            // Rook moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, -1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
            // forward knight hops
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +2, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +2, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -2, +1, +0, +0, 1));
            break;
        case::Blue:
            // Rook moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, -1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
            // King moves
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +0, +0, 1));
            this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +0, +0, 1));
            break;
        }
        break;
    case::Queen:
        // Rook moves
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, -1, +0, 0));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, -1, 0));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +1, +0, 0));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +1, 0));
        // bishop moves
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, -1, -1, 0));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +1, -1, 0));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +1, +1, 0));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, -1, +1, 0));
        break;
    case::King:
        // king moves
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, -1, +0, +0, 1));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, -1, +0, +0, 1));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, -1, +0, +0, 1));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +0, +0, +0, 1));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +1, +1, +0, +0, 1));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), +0, +1, +0, +0, 1));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +1, +0, +0, 1));
        this->mobilities.push_back(new Mobility(AttackMove, DefaultMobilityFlags(), -1, +0, +0, +0, 1));
        // castling
        this->mobilities.push_back(new Mobility(Move, QueenSideCastleFlags(), -1, 0, -1, 0, 3));
        this->mobilities.push_back(new Mobility(Move, KingSideCastleFlags(), +1, 0, +1, 0, 2));
        break;
    }
}

Ghost::Ghost(int x, int y, Piece* parent) {
    this->x = x;
    this->y = y;
    this->parent = parent;
}
