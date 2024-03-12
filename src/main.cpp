#include <uci.h>
#include <simulation.h>
#include <iostream>


void testEngine()
{
    ZobristHashing::init();

    Board board;
    board.initBoard({});

    board.makeMove(4, 1, 4, 3 ); // e2-e4
    board.makeMove(6, 7, 5, 5); // g8-f6

    auto result = SearchManager::calculateBestMove_threads(board, 4, 4, true);
    if (result.first)
        LegalMove bestMove = result.second;
}

void testUCI()
{
    uci api;

    api.parseCommand("uci");
    api.parseCommand("isready");
    api.parseCommand("setoption name WhitePawn value Red");
    api.parseCommand("setoption name WhiteKnight value Blue");
    api.parseCommand("setoption name WhiteBishop value Blue");
    api.parseCommand("setoption name WhiteRook value Blue");
    api.parseCommand("setoption name BlackPawn value Red");
    api.parseCommand("setoption name BlackKnight value Blue");
    api.parseCommand("setoption name BlackBishop value Blue");
    api.parseCommand("setoption name BlackRook value Blue");
    api.parseCommand("ucinewgame");
    api.parseCommand("legalmoves");
    api.parseCommand("position curpos moves d2d4");
    api.parseCommand("go depth 3");
}

void startUCI()
{
    uci api;
    api.run();
}

void startSimulation()
{
    ZobristHashing::init();

    EssenceArgs essenceArgs;
    essenceArgs.whitePawn = Red;
    essenceArgs.whiteKnight = Red;
    essenceArgs.whiteBishop = Red;
    essenceArgs.whiteRook = Red;
    essenceArgs.blackPawn = Blue;
    essenceArgs.blackKnight = Blue;
    essenceArgs.blackBishop = Blue;
    essenceArgs.blackRook = Blue;

    SimulationManager::simulateGames(10, essenceArgs, 4, 8);
}

void startTraining()
{

}

int main()
{
    // startUCI();
    // testUCI();
    // testEngine();
    // startSimulation();
    startTraining();

    return 0;
}
