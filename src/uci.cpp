#include "uci.h"

#include <iostream>
#include <sstream>
#include <vector>


void uci::run()
{
    std::string command;

    bool terminated = false;
    while (!terminated) {
        std::getline(std::cin, command);
        terminated = parseCommand(command);
    }
}

bool uci::parseCommand(std::string command) {
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

        sendMobilities(new Piece(Pawn, White, Classic));
        sendMobilities(new Piece(Pawn, White, Red));
        sendMobilities(new Piece(Pawn, White, Blue));
        sendMobilities(new Piece(Knight, White, Classic));
        sendMobilities(new Piece(Knight, White, Red));
        sendMobilities(new Piece(Knight, White, Blue));
        sendMobilities(new Piece(Bishop, White, Classic));
        sendMobilities(new Piece(Bishop, White, Red));
        sendMobilities(new Piece(Bishop, White, Blue));
        sendMobilities(new Piece(Rook, White, Classic));
        sendMobilities(new Piece(Rook, White, Red));
        sendMobilities(new Piece(Rook, White, Blue));
        sendMobilities(new Piece(Queen, White, Classic));
        sendMobilities(new Piece(King, White, Classic));

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
            whitePawnEssence = stringToEssence(value);
        else if (name == "WhiteRook")
            whiteRookEssence = stringToEssence(value);
        else if (name == "WhiteKnight")
            whiteKnightEssence = stringToEssence(value);
        else if (name == "WhiteBishop")
            whiteBishopEssence = stringToEssence(value);
        else if (name == "BlackPawn")
            blackPawnEssence = stringToEssence(value);
        else if (name == "BlackRook")
            blackRookEssence = stringToEssence(value);
        else if (name == "BlackKnight")
            blackKnightEssence = stringToEssence(value);
        else if (name == "BlackBishop")
            blackBishopEssence = stringToEssence(value);
    }
    else if (tokens[0] == "ucinewgame")
    {
        curGame = new Game(whitePawnEssence, whiteRookEssence, whiteKnightEssence, whiteBishopEssence,
            blackPawnEssence, blackRookEssence, blackKnightEssence, blackBishopEssence);
    }
    else if (tokens[0] == "legalmoves")
    {
        Board* board = curGame->myBoard;

        std::vector<LegalMove*> legalMoves;
        if (board->curTurn == White)
            legalMoves = board->getLegalMoves(White);
        else
            legalMoves = board->getLegalMoves(Black);

        if (!legalMoves.empty())
        {
            std::string legalMovesStr = "";
            for each (LegalMove* legalMove in legalMoves)
                legalMovesStr += " " + legalMoveToString(legalMove);

            std::cout << "legalmoves" + legalMovesStr + '\n';
        }
        else
        {
            if (board->curTurn == White && board->whiteCheck)
                std::cout << "result Black" << std::endl;
            else if (board->curTurn == Black && board->blackCheck)
                std::cout << "result White" << std::endl;
            else
                std::cout << "result Stalemate" << std::endl;
        }
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
                    char promotionChar = move[4] - 32;
                    PieceType promotionType = (PieceType)promotionChar;

                    board->makeMove(sourceX, sourceY, destinationX, destinationY, promotionType);
                }
            }
        }
    }
    else if (tokens[0] == "go")
    {
        Board* board = curGame->myBoard;

        std::vector<LegalMove*> legalMoves = board->getLegalMoves(board->curTurn);
        if (!legalMoves.empty()) {
            LegalMove* bestMove = ai->calculateBestMove(board, 2);
            std::cout << "bestmove " << legalMoveToString(bestMove) << std::endl;
        }
        else
        {
            if (board->curTurn == White && board->whiteCheck)
                std::cout << "result Black" << std::endl;
            else if (board->curTurn == Black && board->blackCheck)
                std::cout << "result White" << std::endl;
            else
                std::cout << "result Stalemate" << std::endl;
        }
    }
    else if (tokens[0] == "quit")
        return true;

    return false;
}

PieceType uci::stringToPieceType(std::string value)
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

std::string uci::pieceTypeToString(PieceType value)
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

Essence uci::stringToEssence(std::string value)
{
    if (value == "Red")
        return Red;
    else if (value == "Blue")
        return Blue;
    else
        return Classic;
}

std::string uci::essenceToString(Essence value)
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

void uci::sendMobilities(Piece* curPiece)
{
    std::string pieceType = pieceTypeToString(curPiece->type);
    std::string essence = essenceToString(curPiece->essence);

    for each (Mobility * curMobility in curPiece->getMobilities())
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

std::string uci::legalMoveToString(LegalMove* value)
{

    std::string retValue = "";
    retValue += 'a' + value->x1;
    retValue += '0' + 7 - value->y1 + 1;
    retValue += 'a' + value->x2;
    retValue += '0' + 7 - value->y2 + 1;

    /*
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
    */

    return retValue;
}
