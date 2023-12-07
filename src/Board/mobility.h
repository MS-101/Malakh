#ifndef MOBILITY_H
#define MOBILITY_H

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

struct MobilityFlags {
    bool hasty = false;
    bool vigilant = false;
    bool initiative = false;
    bool uninterruptible = false;
    bool cowardly = false;
    bool inspiring = false;

    int affected_x;
    int affected_y;

    MobilityFlags setHasty() {
        hasty = true;

        return *this;
    }

    MobilityFlags setVigilant() {
        vigilant = true;

        return *this;
    }

    MobilityFlags setInitiative() {
        initiative = true;

        return *this;
    }

    MobilityFlags setUninterruptible() {
        uninterruptible = true;

        return *this;
    }

    MobilityFlags setCowardly() {
        cowardly = true;

        return *this;
    }

    MobilityFlags setInspiring(int affected_x, int affected_y) {
        inspiring = true;
        this->affected_x = affected_x;
        this->affected_y = affected_y;

        return *this;
    }
};

class Mobility {
public:
    Mobility(MovementType type, MobilityFlags flags, int start_x, int start_y, int direction_x, int direction_y, int limit);

    MovementType type;
    MobilityFlags flags;
    int start_x, start_y, direction_x, direction_y, limit;
};

#endif