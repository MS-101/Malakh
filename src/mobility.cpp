#include "mobility.h"

std::string LegalMove::toString()
{
    std::string value;

    switch (castling) {
    case kingSide:
        value = "o-o";
        break;
    case queenSide:
        value = "o-o-o";
        break;
    default:
        value.push_back('a' + x1);
        value.push_back('1' + y1);
        value.push_back('a' + x2);
        value.push_back('1' + y2);
        break;
    }

    return value;
}

std::vector<Mobility> mobilityConfig[6][3] = {
    { // Pawn
        { // Classic
            {Move, +0, +1, +0, +0, 1},
            {Move, +0, +1, +0, +1, 2, {true, true, true, false}},
            {Attack, -1, +1, +0, +0, 1, {false, false, false, true}},
            {Attack, +1, +1, +0, +0, 1, {false, false, false, true}}
        },
        { // Red
            {Move, +0, +1, +0, +1, 2},
            {AttackMove, -1, +1, +0, +0, 1, {false, false, false, true}},
            {AttackMove, +1, +1, +0, +0, 1, {false, false, false, true}}
        },
        { //Blue
            // pawn moves
            {Move, +0, +1, +0, +0, 1},
            {Attack, -1, +1, +0, +0, 1, {false, false, false, true}},
            {Attack, +1, +1, +0, +0, 1, {false, false, false, true}},
            // backward pawn moves
            {Move, +0, -1, +0, +0, 1},
            {Attack, -1, -1, +0, +0, 1, {false, false, false, true}},
            {Attack, +1, -1, +0, +0, 1, {false, false, false, true}}
        }
    },
    { // Rook
        { // Classic
            {AttackMove, -1, +0, -1, +0, 0},
            {AttackMove, +0, -1, +0, -1, 0},
            {AttackMove, +1, +0, +1, +0, 0},
            {AttackMove, +0, +1, +0, +1, 0}
        },
        { // Red
            // Rook moves
            {AttackMove, -1, +0, -1, +0, 0},
            {AttackMove, +0, -1, +0, -1, 0},
            {AttackMove, +1, +0, +1, +0, 0},
            {AttackMove, +0, +1, +0, +1, 0},
            // forward knight hops
            {AttackMove, +2, +1, +0, +0, 1},
            {AttackMove, +1, +2, +0, +0, 1},
            {AttackMove, -1, +2, +0, +0, 1},
            {AttackMove, -2, +1, +0, +0, 1},
        },
        { // Blue
            // Rook moves
            {AttackMove, -1, +0, -1, +0, 0},
            {AttackMove, +0, -1, +0, -1, 0},
            {AttackMove, +1, +0, +1, +0, 0},
            {AttackMove, +0, +1, +0, +1, 0},
            // King moves
            {AttackMove, -1, -1, +0, +0, 1},
            {AttackMove, -1, +1, +0, +0, 1},
            {AttackMove, +1, -1, +0, +0, 1},
            {AttackMove, +1, +1, +0, +0, 1}
        }
    },
    { // Knight
        { // Classic
            {AttackMove, -2, -1, +0, +0, 1},
            {AttackMove, -1, -2, +0, +0, 1},
            {AttackMove, +1, -2, +0, +0, 1},
            {AttackMove, +2, -1, +0, +0, 1},
            {AttackMove, +2, +1, +0, +0, 1},
            {AttackMove, +1, +2, +0, +0, 1},
            {AttackMove, -1, +2, +0, +0, 1},
            {AttackMove, -2, +1, +0, +0, 1}
        },
        { // Red
            // knight hops
            {AttackMove, -2, -1, +0, +0, 1},
            {AttackMove, -1, -2, +0, +0, 1},
            {AttackMove, +1, -2, +0, +0, 1},
            {AttackMove, +2, -1, +0, +0, 1},
            {AttackMove, +2, +1, +0, +0, 1},
            {AttackMove, +1, +2, +0, +0, 1},
            {AttackMove, -1, +2, +0, +0, 1},
            {AttackMove, -2, +1, +0, +0, 1},
            // additional hops
            {AttackMove, -3, +0, +0, +0, 1},
            {AttackMove, +3, +0, +0, +0, 1},
            {AttackMove, +0, -3, +0, +0, 1},
            {AttackMove, +0, +3, +0, +0, 1},
        },
        { // Blue
            // knight hops
            {AttackMove, -2, -1, +0, +0, 1},
            {AttackMove, -1, -2, +0, +0, 1},
            {AttackMove, +1, -2, +0, +0, 1},
            {AttackMove, +2, -1, +0, +0, 1},
            {AttackMove, +2, +1, +0, +0, 1},
            {AttackMove, +1, +2, +0, +0, 1},
            {AttackMove, -1, +2, +0, +0, 1},
            {AttackMove, -2, +1, +0, +0, 1},
            // king moves
            {AttackMove, -1, -1, +0, +0, 1},
            {AttackMove, +0, -1, +0, +0, 1},
            {AttackMove, +1, -1, +0, +0, 1},
            {AttackMove, +1, +0, +0, +0, 1},
            {AttackMove, +1, +1, +0, +0, 1},
            {AttackMove, +0, +1, +0, +0, 1},
            {AttackMove, -1, +1, +0, +0, 1},
            {AttackMove, -1, +0, +0, +0, 1}
        }
    },
    { // Bishop
        { // Classic
            {AttackMove, -1, -1, -1, -1, 0},
            {AttackMove, +1, -1, +1, -1, 0},
            {AttackMove, +1, +1, +1, +1, 0},
            {AttackMove, -1, +1, -1, +1, 0}
        },
        { // Red
            // bishop moves
            {AttackMove, -1, -1, -1, -1, 0},
            {AttackMove, +1, -1, +1, -1, 0},
            {AttackMove, +1, +1, +1, +1, 0},
            {AttackMove, -1, +1, -1, +1, 0},
            // forward knight hops
            {AttackMove, +2, +1, +0, +0, 1},
            {AttackMove, +1, +2, +0, +0, 1},
            {AttackMove, -1, +2, +0, +0, 1},
            {AttackMove, -2, +1, +0, +0, 1}
        },
        { // Blue
            // bishop moves
            {AttackMove, -1, -1, -1, -1, 0},
            {AttackMove, +1, -1, +1, -1, 0},
            {AttackMove, +1, +1, +1, +1, 0},
            {AttackMove, -1, +1, -1, +1, 0},
            /// king moves
            {AttackMove, +0, -1, +0, +0, 1},
            {AttackMove, -1, +0, +0, +0, 1},
            {AttackMove, +1, +0, +0, +0, 1},
            {AttackMove, +0, +1, +0, +0, 1}
        }
    },
    { // Queen
        { // Classic
            // Rook moves
            {AttackMove, -1, +0, -1, +0, 0},
            {AttackMove, +0, -1, +0, -1, 0},
            {AttackMove, +1, +0, +1, +0, 0},
            {AttackMove, +0, +1, +0, +1, 0},
            // bishop moves
            {AttackMove, -1, -1, -1, -1, 0},
            {AttackMove, +1, -1, +1, -1, 0},
            {AttackMove, +1, +1, +1, +1, 0},
            {AttackMove, -1, +1, -1, +1, 0}
        }
    },
    { // King
        { // Classic
            {AttackMove, -1, -1, +0, +0, 1},
            {AttackMove, +0, -1, +0, +0, 1},
            {AttackMove, +1, -1, +0, +0, 1},
            {AttackMove, +1, +0, +0, +0, 1},
            {AttackMove, +1, +1, +0, +0, 1},
            {AttackMove, +0, +1, +0, +0, 1},
            {AttackMove, -1, +1, +0, +0, 1},
            {AttackMove, -1, +0, +0, +0, 1},
        }
    }
};
