#include "uci.h"

#include <iostream>
#include <sstream>
#include <vector>


void uci::Run()
{
    std::string command;

    bool terminated = false;
    while (!terminated) {
        std::getline(std::cin, command);
        terminated = ParseCommand(command);
    }
}

bool uci::ParseCommand(std::string command) {
    std::stringstream ssCommand(command);

    std::vector<std::string> tokens;
    std::string curToken;
    while (std::getline(ssCommand, curToken, ' '))
        tokens.push_back(curToken);

    if (tokens[0] == "uci")
    {
        std::cout << "id name Malakh\n";
        std::cout << "id author Martin Svab\n";

        std::cout << "option name WhitePawn type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name WhiteRook type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name WhiteKnight type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name WhiteBishop type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackPawn type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackRook type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackKnight type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackBishop type combo default Classic var Classic var Red var Blue\n";

        SendMobilities(new Piece(Pawn, White, Classic));
        SendMobilities(new Piece(Pawn, White, Red));
        SendMobilities(new Piece(Pawn, White, Blue));
        SendMobilities(new Piece(Knight, White, Classic));
        SendMobilities(new Piece(Knight, White, Red));
        SendMobilities(new Piece(Knight, White, Blue));
        SendMobilities(new Piece(Bishop, White, Classic));
        SendMobilities(new Piece(Bishop, White, Red));
        SendMobilities(new Piece(Bishop, White, Blue));
        SendMobilities(new Piece(Rook, White, Classic));
        SendMobilities(new Piece(Rook, White, Red));
        SendMobilities(new Piece(Rook, White, Blue));
        SendMobilities(new Piece(Queen, White, Classic));
        SendMobilities(new Piece(King, White, Classic));

        std::cout << "uciok\n";
    }
    else if (tokens[0] == "isready")
    {
        std::cout << "readyok\n";
    }
    else if (tokens[0] == "setoption")
    {
        std::string name = tokens[2];
        std::string value = tokens[4];

        if (name == "WhitePawn")
            whitePawnEssence = StringToEssence(value);
        else if (name == "WhiteRook")
            whiteRookEssence = StringToEssence(value);
        else if (name == "WhiteKnight")
            whiteKnightEssence = StringToEssence(value);
        else if (name == "WhiteBishop")
            whiteBishopEssence = StringToEssence(value);
        else if (name == "BlackPawn")
            blackPawnEssence = StringToEssence(value);
        else if (name == "BlackRook")
            blackRookEssence = StringToEssence(value);
        else if (name == "BlackKnight")
            blackKnightEssence = StringToEssence(value);
        else if (name == "BlackBishop")
            blackBishopEssence = StringToEssence(value);
    }
    else if (tokens[0] == "ucinewgame")
    {
        curGame = new Game(whitePawnEssence, whiteRookEssence, whiteKnightEssence, whiteBishopEssence,
            blackPawnEssence, blackRookEssence, blackKnightEssence, blackBishopEssence);
    }
    else if (tokens[0] == "legalmoves")
    {
        Board* board = curGame->myBoard;

        std::list<LegalMove*> legalMoves;
        if (tokens[1] == "White")
            legalMoves = board->GetLegalMoves(White);
        else if (tokens[1] == "Black")
            legalMoves = board->GetLegalMoves(Black);

        std::string legalMovesStr = "";
        for each (LegalMove* legalMove in legalMoves)
            legalMovesStr += " " + LegalMoveToString(legalMove);

        std::cout << "legalmoves" + legalMovesStr + '\n';
    }
    else if (tokens[0] == "position")
    {
        Board* board = curGame->myBoard;

        if (tokens[1] == "curpos")
        {
            if (tokens[2] == "moves")
            {
                for (int i = 3; i < tokens.size(); i++)
                {
                    std::string move = tokens[i];

                    int sourceX = move[0] - 'a';
                    int sourceY = 7 - (move[1] - '1');
                    int destinationX = move[2] - 'a';
                    int destinationY = 7 - (move[3] - '1');
                    char promotionChar = move[4] + 32;
                    PieceType promotionType = (PieceType)promotionChar;

                    board->PerformMove(sourceX, sourceY, destinationX, destinationY, promotionType);
                }
            }
        }
    }
    else if (tokens[0] == "go")
    {
        Board* board = curGame->myBoard;

        std::list<LegalMove*> legalMoves = board->GetLegalMoves(board->curTurn);

        std::srand(std::time(0));
        if (!legalMoves.empty()) {
            int randomIndex = std::rand() % legalMoves.size();

            auto it = legalMoves.begin();
            std::advance(it, randomIndex);
            LegalMove* randomMove = *it;

            std::cout << "bestmove " << LegalMoveToString(randomMove) << std::endl;
        }
    }
    else if (tokens[0] == "quit")
        return true;

    return false;
}

PieceType uci::StringToPieceType(std::string value)
{
    if (value == "Knight")
        return Knight;
    else if (value == "Bishop")
        return Bishop;
    else if (value == "Rook")
        return Rook;
    else if (value == "Queen")
        return Queen;
    else if (value == "King")
        return King;
    else
        return Pawn;
}

std::string uci::PieceTypeToString(PieceType value)
{
    switch (value) {
    case::Knight:
        return "Knight";
        break;
    case::Bishop:
        return "Bishop";
        break;
    case::Rook:
        return "Rook";
        break;
    case::Queen:
        return "Queen";
        break;
    case::King:
        return "King";
        break;
    default:
        return "Pawn";
        break;
    }
}

Essence uci::StringToEssence(std::string value)
{
    if (value == "Red")
        return Red;
    else if (value == "Blue")
        return Blue;
    else
        return Classic;
}

std::string uci::EssenceToString(Essence value)
{
    switch (value) {
        case::Red:
            return "Red";
            break;
        case::Blue:
            return "Blue";
            break;
        default:
            return "Classic";
            break;
    }
}

void uci::SendMobilities(Piece* curPiece)
{
    std::string pieceType = PieceTypeToString(curPiece->type);
    std::string essence = EssenceToString(curPiece->essence);

    for each (Mobility * curMobility in curPiece->mobilities)
    {
        MobilityFlags flags = curMobility->flags;
        if (!flags.initiative && !flags.cowardly && !flags.hasty && !flags.inspiring && !flags.uninterruptible)
        {
            std::string movementType = "";
            switch (curMobility->type)
            {
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

            std::cout << "mobility " + pieceType + " " + essence + " " + movementType
                + " " + std::to_string(curMobility->start_x) + " " + std::to_string(curMobility->start_y)
                + " " + std::to_string(curMobility->direction_x) + " " + std::to_string(curMobility->direction_y)
                + " " + std::to_string(curMobility->limit) + "\n";
        }
    }
}

std::string uci::LegalMoveToString(LegalMove* value)
{
    Piece* piece = value->pieceMovement->piece;
    Movement* movement = value->pieceMovement->movement;

    std::string retValue = "";
    retValue += 'a' + piece->x;
    retValue += '0' + 7 - piece->y + 1;
    retValue += 'a' + movement->x;
    retValue += '0' + 7 - movement->y + 1;

    if (movement->mobility->flags.hasty)
    {
        int hastyX = movement->x;
        int hastyY = movement->y;

        if (piece->color == White)
        {
            hastyX -= movement->mobility->direction_x;
            hastyY += movement->mobility->direction_y;
        }
        else
        {
            hastyX += movement->mobility->direction_x;
            hastyY -= movement->mobility->direction_y;
        }

        retValue += "_H";
        retValue += 'a' + hastyX;
        retValue += '0' + 7 - hastyY + 1;
    }

    if (movement->mobility->flags.vigilant)
    {
        retValue += "_V";
    }

    if (movement->mobility->flags.inspiring)
    {
        int inspiringX1 = piece->x + movement->mobility->flags.affected_x;
        int inspiringY1 = piece->y;
        int inspiringX2 = movement->x;
        int inspiringY2 = movement->y;

        if (piece->color == White)
        {
            inspiringY1 -= movement->mobility->flags.affected_y;
            inspiringX2 -= movement->mobility->direction_x;
            inspiringY2 += movement->mobility->direction_y;
        }
        else
        {
            inspiringY1 += movement->mobility->flags.affected_y;
            inspiringX2 += movement->mobility->direction_x;
            inspiringY2 -= movement->mobility->direction_y;
        }

        retValue += "_I";
        retValue += 'a' + inspiringX1;
        retValue += '0' + 7 - inspiringY1 + 1;
        retValue += 'a' + inspiringX2;
        retValue += '0' + 7 - inspiringY2 + 1;
    }

    return retValue;
}
