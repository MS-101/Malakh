#include <mobility.h>
#include <iostream>


LegalMove::LegalMove() {}

LegalMove::LegalMove(int x1, int y1, int x2, int y2, Mobility mobility)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->mobility = mobility;
}

LegalMove::LegalMove(std::string value)
{
    this->x1 = value[0] - 'a';
    this->y1 = value[1] - '1';
    this->x2 = value[2] - 'a';
    this->y2 = value[3] - '1';
}

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

std::string LegalMove::toStringWithFlags(PieceColor color)
{
    std::string value = toString();

    if (mobility.flags.hasty) {
        value += "_H";

        int hastyX = x2;
        int hastyY = y2;
        if (color == White) {
            hastyX -= mobility.direction_x;
            hastyY -= mobility.direction_y;
        }
        else {
            hastyX += mobility.direction_x;
            hastyY += mobility.direction_y;
        }

        value.push_back('a' + hastyX);
        value.push_back('1' + hastyY);
    }

    if (mobility.flags.vigilant)
        value += "_V";

    return value;
}

std::vector<Mobility> Mobilities::mobilityConfig[6][3] = {
    { // Pawn
        { // Classic
            {Move, +0, +1, +0, +0, 1},
            {Attack, -1, +1, +0, +0, 1},
            {Attack, +1, +1, +0, +0, 1}
        },
        { // Red
            {Move, +0, +1, +0, +1, 2},
            {AttackMove, -1, +1, +0, +0, 1},
            {AttackMove, +1, +1, +0, +0, 1}
        },
        { //Blue
            // pawn moves
            {Move, +0, +1, +0, +0, 1},
            {Attack, -1, +1, +0, +0, 1},
            {Attack, +1, +1, +0, +0, 1},
            // backward pawn moves
            {Move, +0, -1, +0, +0, 1},
            {Attack, -1, -1, +0, +0, 1},
            {Attack, +1, -1, +0, +0, 1}
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
