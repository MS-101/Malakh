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
* Uninterruptibe creates a chain where every move is ilegal except the last move. If the movement is limited only the move on that limit can be legal. If the movement is unlimited no move can be legal. // DONE
* Initiative moves can only be performed only if the piece has not moved yet. // DONE
* Cowardly is a move that can be cut by hostile attacks (if piece itself is attacked, cowardly moves cannot be performed). // DONE
* Inspiring can only be performed if piece that this move is affecting has not moved yet. After this move is performed place the affected piece behind it.
* Hasty leaves a "ghost" piece behind itself. This piece can only be attacked by "observant" hostile moves.
* Vigilant can capture "ghost" pieces left behind by "hasty" moves.
*/

class MobilityFlags {
    public:
        MobilityFlags(bool uninterruptible, bool initiative, bool cowardly, bool inspiring, bool hasty, bool vigilant, int affected_x, int affected_y);

        bool uninterruptible = false;
        bool initiative = false;
        bool cowardly = false;
        bool inspiring = false;
        bool hasty = false;
        bool vigilant = false;

        int affected_x;
        int affected_y;
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