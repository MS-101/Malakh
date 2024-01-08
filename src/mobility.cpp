#include "mobility.h"
#include <iostream>

std::string LegalMove::toString(PieceColor color)
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

        if (mobility.flags.hasty) {
            value += "_H";

            int hastyX = x2;
            int hastyY = y2;
            if (color == White) {
                hastyX -= mobility.direction_x;
                hastyY -= mobility.direction_y;
            } else {
                hastyX += mobility.direction_x;
                hastyY += mobility.direction_y;
            }

            value.push_back('a' + hastyX);
            value.push_back('1' + hastyY);
        }
            

        if (mobility.flags.vigilant)
            value += "_V";

        break;
    }

    return value;
}

std::vector<Mobility> Mobilities::mobilityConfig[6][3] = {
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

void Mobilities::printMobilities(PieceType type, PieceEssence essence)
{
    for (Mobility& mobility : mobilityConfig[type][essence]) {
        std::string movementType = "";
        switch (mobility.type) {
        case::Move:
            movementType = "Move";
            break;
        case::Attack:
            movementType = "Attack";
            break;
        case::AttackMove:
            movementType = "AttackMove";
            break;
        }

        std::cout << "mobility " << typeToString(type) << " " << essenceToString(essence) << " " << movementType
            << " " << mobility.start_x << " " << mobility.start_y
            << " " << mobility.direction_x << " " << mobility.direction_y
            << " " << mobility.limit << "\n";
    }
}
