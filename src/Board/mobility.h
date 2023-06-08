#ifndef MOBILITY_H
#define MOBILITY_H

#include <list>

/*
* Move can only move to empty squares.
* Attack can only capture hostile pieces.
* AttackMove can move to empty squares and capture hostile pieces.
*/

enum MovementType { Move, Attack, AttackMove };

/*
* OnlyLastIsValid creates a chain where every move is ilegal except the last move. If the movement is limited only the move on that limit can be legal. If the movement is unlimited no move can be legal.
* Initiative moves can only be performed only if the piece has not moved yet.
* Cowardly is a move that can be cut by hostile attacks (if piece itself is attacked, cowardly moves will not be calculated at all).
* CallQueenRook can be only performed if queen rook has not moved yet. After performing this move the rook is placed behind itself.
* CallKingRook can be only performed if king rook has not moved yet. After performing this move the rook is placed behind itself.
* Hasty leaves a "ghost" piece behind itself. This piece can only be attacked by "observant" hostile moves.
* Vigilant can capture "ghost" pieces left behind by "hasty" moves.
*/

class MobilityFlags {
    public:
        MobilityFlags(bool onlyLastIsValid, bool initiative, bool cowardly, bool callQueenRook, bool callKingRook, bool hasty, bool vigilant);

        bool onlyLastIsValid = false;
        bool initiative = false;
        bool cowardly = false; 
        bool callQueenRook = false; 
        bool callKingRook = false; 
        bool hasty = false; 
        bool vigilant = false;
};

class DefaultMobilityFlags : public MobilityFlags {
    public:
        DefaultMobilityFlags();
};

class QueenSideCastleFlags : public MobilityFlags {
    public:
        QueenSideCastleFlags();
};

class KingSideCastleFlags : public MobilityFlags {
    public:
        KingSideCastleFlags();
};

class EnPassantMoveFlags : public MobilityFlags {
    public:
        EnPassantMoveFlags();
};

class EnPassantCaptureFlags : public MobilityFlags {
    public:
        EnPassantCaptureFlags();
};

class Mobility {
    public:
        Mobility(MovementType type, MobilityFlags flags, int start_x, int start_y, int direction_x, int direction_y, int limit);

        MovementType type;
        MobilityFlags flags;
        int start_x, start_y, direction_x, direction_y, limit;
};

#endif