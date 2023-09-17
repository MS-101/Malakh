#include <iostream>
#include "src/game.h"
#include "src/Pieces/pieces.h"
#include <vector>
#include <sstream>

void SendMobilities(Piece* curPiece)
{
    std::string pieceType = curPiece->name;

    std::string essence = "";
    switch (curPiece->essence)
    {
        case::Classic:
            essence = "Classic";
            break;
        case::Red:
            essence = "Red";
            break;
        case::Blue:
            essence = "Blue";
            break;
    }

    for each (Mobility* curMobility in curPiece->mobilities)
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

            std::cout << "info string #MOBILITY " + pieceType + " " + essence + " " + movementType
                + " " + std::to_string(curMobility->start_x) + " " + std::to_string(curMobility->start_y)
                + " " + std::to_string(curMobility->direction_x) + " " + std::to_string(curMobility->direction_y)
                + " " + std::to_string(curMobility->limit) + "\n";
        }
    }
}

void ParseUCICommand(std::string command) {
    std::stringstream ssCommand(command);

    std::vector<std::string> tokens;
    std::string curToken;
    while (std::getline(ssCommand, curToken, ' '))
        tokens.push_back(curToken);

    if (tokens[0] == "uci")
    {
        std::cout << "id name Malakh\n";
        std::cout << "id author Martin Šváb\n";

        std::cout << "option name PlayerPawn type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name PlayerRook type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name PlayerKnight type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name PlayerBishop type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name MalakhPawn type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name MalakhRook type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name MalakhKnight type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name MalakhBishop type combo default Classic var Classic var Red var Blue\n";

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
}

int main() {
    Game* newGame = new Game();
    Board* board = newGame->myBoard;

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        ParseUCICommand(input);
    }

    /*
    Game* newGame = new Game();
    Board* board = newGame->myBoard;

    FILE* myFile = freopen("output.txt", "w", stdout);

    board->InitBoard(Classic, Classic, Classic, Classic, Classic, Classic, Classic, Classic);
    board->PrintBoard();
    board->PrintMoves();

    std::cout << "Performing move...\n\n";

    board->PerformMove(1, 6, 1, 4);
    board->PrintBoard();
    board->PrintMoves();
    */

    return 0;
}
