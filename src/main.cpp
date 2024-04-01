#include <uci.h>
#include <simulation.h>
#include <training.h>
#include <iostream>
#


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

    int gameCount = 10;
    int malakhDepth = 4;
    int fairyStockfishDepth = 6;

    EssenceArgs redEssenceConfig;
    redEssenceConfig.whitePawn = Red;
    redEssenceConfig.whiteKnight = Red;
    redEssenceConfig.whiteBishop = Red;
    redEssenceConfig.whiteRook = Red;
    redEssenceConfig.blackPawn = Red;
    redEssenceConfig.blackKnight = Red;
    redEssenceConfig.blackBishop = Red;
    redEssenceConfig.blackRook = Red;

    SimulationManager::simulateGames(gameCount, redEssenceConfig, malakhDepth, fairyStockfishDepth);

    EssenceArgs blueEssenceConfig;
    redEssenceConfig.whitePawn = Blue;
    redEssenceConfig.whiteKnight = Blue;
    redEssenceConfig.whiteBishop = Blue;
    redEssenceConfig.whiteRook = Blue;
    redEssenceConfig.blackPawn = Blue;
    redEssenceConfig.blackKnight = Blue;
    redEssenceConfig.blackBishop = Blue;
    redEssenceConfig.blackRook = Blue;

    SimulationManager::simulateGames(gameCount, blueEssenceConfig, malakhDepth, fairyStockfishDepth);
}

void startTraining()
{
    int epochs = 10;

    TrainingManager::trainModel("training/output.csv", "training/output.pt", epochs);
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
