#include <uci.h>
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

    if (tokens[0] == "uci") {
        std::cout << "id name Malakh\n";
        std::cout << "id author Martin Svab\n";

        std::cout << "option name AI type combo default Basic var Basic var Ensemble\n";
        std::cout << "option name WhitePawn type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name WhiteRook type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name WhiteKnight type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name WhiteBishop type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackPawn type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackRook type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackKnight type combo default Classic var Classic var Red var Blue\n";
        std::cout << "option name BlackBishop type combo default Classic var Classic var Red var Blue\n";

        Mobilities::printMobilities(Pawn, Classic);
        Mobilities::printMobilities(Pawn, Red);
        Mobilities::printMobilities(Pawn, Blue);
        Mobilities::printMobilities(Knight, Classic);
        Mobilities::printMobilities(Knight, Red);
        Mobilities::printMobilities(Knight, Blue);
        Mobilities::printMobilities(Bishop, Classic);
        Mobilities::printMobilities(Bishop, Red);
        Mobilities::printMobilities(Bishop, Blue);
        Mobilities::printMobilities(Rook, Classic);
        Mobilities::printMobilities(Rook, Red);
        Mobilities::printMobilities(Rook, Blue);
        Mobilities::printMobilities(Queen, Classic);
        Mobilities::printMobilities(King, Classic);

        std::cout << "uciok\n";
    } else if (tokens[0] == "isready") {
        std::cout << "readyok\n";
    } else if (tokens[0] == "setoption") {
        std::string name = tokens[2];
        std::string value = tokens[4];

        if (name == "WhitePawn")
            essenceConfig.whitePawn = stringToEssence(value);
        else if (name == "WhiteRook")
            essenceConfig.whiteRook = stringToEssence(value);
        else if (name == "WhiteKnight")
            essenceConfig.whiteKnight = stringToEssence(value);
        else if (name == "WhiteBishop")
            essenceConfig.whiteBishop = stringToEssence(value);
        else if (name == "BlackPawn")
            essenceConfig.blackPawn = stringToEssence(value);
        else if (name == "BlackRook")
            essenceConfig.blackRook = stringToEssence(value);
        else if (name == "BlackKnight")
            essenceConfig.blackKnight = stringToEssence(value);
        else if (name == "BlackBishop")
            essenceConfig.blackBishop = stringToEssence(value);
        else if (name == "AI") {
            if (value == "Basic")
                useEnsemble = false;
            else if (value == "Ensemble")
                useEnsemble = true;
        }
    } else if (tokens[0] == "ucinewgame") {
        board.initBoard(essenceConfig);
    } else if (tokens[0] == "legalmoves") {
        GameResult gameResult = board.getResult();

        switch (gameResult) {
        case Unresolved: {
            std::vector<LegalMove> legalMoves = board.getLegalMoves();

            std::string legalMovesStr = "";
            for (LegalMove& legalMove : legalMoves)
                legalMovesStr += " " + legalMove.toStringWithFlags(board.curTurn);

            std::cout << "legalmoves" + legalMovesStr + '\n';
            break;
        }
        case WhiteWin:
            std::cout << "result White" << std::endl;
            break;
        case BlackWin:
            std::cout << "result Black" << std::endl;
            break;
        case Stalemate:
            std::cout << "result Stalemate" << std::endl;
            break;
        }
    } else if (tokens[0] == "position") {
        if (tokens[1] == "curpos") {
            if (tokens[2] == "moves") {
                for (int i = 3; i < tokens.size(); i++) {
                    std::string move = tokens[i];

                    int sourceX = move[0] - 'a';
                    int sourceY = move[1] - '1';
                    int destinationX = move[2] - 'a';
                    int destinationY = move[3] - '1';

                    PieceType promotionType = Pawn;
                    if (move.length() == 5) {
                        char promotionChar = move[4];
                        switch (promotionChar) {
                        case 'q':
                            promotionType = Queen;
                            break;
                        case 'r':
                            promotionType = Rook;
                            break;
                        case 'b':
                            promotionType = Bishop;
                            break;
                        case 'n':
                            promotionType = Knight;
                            break;
                        }
                    }

                    board.makeMove(sourceX, sourceY, destinationX, destinationY, promotionType);

                    if ((board.pieces[getPieceIndex(White, King)].value & board.attacks[Black].value)
                        || (board.pieces[getPieceIndex(Black, King)].value & board.attacks[White].value))
                        std::cout << "check" << std::endl;
                }
            }
        }
    } else if (tokens[0] == "go") {
        if (tokens[1] == "depth") {
            int depth = stoi(tokens[2]);

            GameResult gameResult = board.getResult();
            switch (gameResult) {
            case Unresolved: {
                auto result = SearchManager::calculateBestMove_threads(board, depth, 4, useEnsemble, false);
                if (result.first) {
                    LegalMove bestMove = result.second;
                    std::cout << "bestmove " << bestMove.toStringWithFlags(board.curTurn) << std::endl;
                }
                break;
            }
            case WhiteWin:
                std::cout << "result White" << std::endl;
                break;
            case BlackWin:
                std::cout << "result Black" << std::endl;
                break;
            case Stalemate:
                std::cout << "result Stalemate" << std::endl;
                break;
            }
        }
    } else if (tokens[0] == "quit") {
        return true;
    }

    return false;
}
