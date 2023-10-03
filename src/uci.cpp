#include "uci.h"
#include "Pieces/pieces.h"
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

        SendMobilities(new Pawn(White, Classic));
        SendMobilities(new Pawn(White, Red));
        SendMobilities(new Pawn(White, Blue));
        SendMobilities(new Knight(White, Classic));
        SendMobilities(new Knight(White, Red));
        SendMobilities(new Knight(White, Blue));
        SendMobilities(new Bishop(White, Classic));
        SendMobilities(new Bishop(White, Red));
        SendMobilities(new Bishop(White, Blue));
        SendMobilities(new Rook(White, Classic));
        SendMobilities(new Rook(White, Red));
        SendMobilities(new Rook(White, Blue));

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

        std::string availableMoves = "";
        if (tokens[1] == "White")
            for each (Piece * whitePiece in board->whitePieces)
                availableMoves += GetAvailableMoves(whitePiece);
        else if (tokens[1] == "Black")
            for each (Piece * blackPiece in board->blackPieces)
                availableMoves += GetAvailableMoves(blackPiece);

        std::cout << "legalmoves" + availableMoves + '\n';
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

                    board->PerformMove(sourceX, sourceY, destinationX, destinationY);
                }
            }
        }
    }
    else if (tokens[0] == "go")
    {
        Board* board = curGame->myBoard;

        std::string availableMoves = "";
        if (board->curTurn == White)
        {
            for each (Piece * whitePiece in board->whitePieces)
                availableMoves += GetAvailableMoves(whitePiece);
        }
        else
        {
            for each (Piece * blackPiece in board->blackPieces)
                availableMoves += GetAvailableMoves(blackPiece);
        }

        std::istringstream movesStream(availableMoves);
        std::vector<std::string> moveTokens;
        std::string move;

        while (std::getline(movesStream, move, ' ')) {
            moveTokens.push_back(move);
        }

        std::srand(std::time(0));
        if (!moveTokens.empty()) {
            int randomIndex = std::rand() % moveTokens.size();
            std::string randomMove = moveTokens[randomIndex];

            std::cout << "bestmove " << randomMove << std::endl;
        }
    }
    else if (tokens[0] == "quit")
        return true;

    return false;
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
    switch (value)
    {
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
    std::string pieceType = curPiece->name;
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

std::string uci::GetAvailableMoves(Piece* curPiece)
{
    std::string value = "";

    std::list<Movement*> availableMoves = curPiece->availableMoves;
    for (auto movementIterator = availableMoves.begin();
        movementIterator != availableMoves.end(); ++movementIterator) {
        Movement* curMove = *movementIterator;

        while (curMove != nullptr) {
            if (curMove->legal)
            {
                std::string newValue = " ";
                newValue += 'a' + curPiece->x;
                newValue += '0' + 7 - curPiece->y + 1;
                newValue += 'a' + curMove->x;
                newValue += '0' + 7 - curMove->y + 1;

                value += newValue;
            }

            curMove = curMove->next;
        }
    }

    return value;
}

