#include <iostream>
#include <uci.h>
#include <simulation.h>
#include <database.h>


void testEngine()
{
    Board board;
    board.initBoard({});

    board.makeMove(4, 1, 4, 3 ); // e2-e4
    board.makeMove(6, 7, 5, 5); // g8-f6

    auto result = SearchManager::calculateBestMove_threads(board, 4, 4, true, false);
    if (result.first)
        LegalMove bestMove = result.second;
}

void testUCI()
{
    uci api;

    api.parseCommand("uci");
    api.parseCommand("isready");
    api.parseCommand("setoption name AI value Ensemble");
    api.parseCommand("setoption name WhitePawn value Red");
    api.parseCommand("setoption name WhiteKnight value Red");
    api.parseCommand("setoption name WhiteBishop value Red");
    api.parseCommand("setoption name WhiteRook value Red");
    api.parseCommand("setoption name BlackPawn value Red");
    api.parseCommand("setoption name BlackKnight value Red");
    api.parseCommand("setoption name BlackBishop value Red");
    api.parseCommand("setoption name BlackRook value Red");
    api.parseCommand("ucinewgame");

    api.parseCommand("go depth 3");
}

void testEnsemble()
{
    Board board;

    EssenceArgs essenceArgs;
    essenceArgs.whitePawn = Red;
    essenceArgs.whiteKnight = Red;
    essenceArgs.whiteBishop = Red;
    essenceArgs.whiteRook = Blue;
    essenceArgs.blackPawn = Red;
    essenceArgs.blackKnight = Red;
    essenceArgs.blackBishop = Red;
    essenceArgs.blackRook = Blue;
    board.initBoard(essenceArgs);

    Ensemble ensemble;

    int* inputArray = board.getInputArray();
    double value = ensemble.forward(inputArray, board.essenceCounts);
    delete[] inputArray;

    std::cout << value << std::endl;
}

void startUCI()
{
    uci api;
    api.run();
}

void startSimulation()
{
    int gameCount = 200;
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

    /*
    EssenceArgs blueEssenceConfig;
    blueEssenceConfig.whitePawn = Blue;
    blueEssenceConfig.whiteKnight = Blue;
    blueEssenceConfig.whiteBishop = Blue;
    blueEssenceConfig.whiteRook = Blue;
    blueEssenceConfig.blackPawn = Blue;
    blueEssenceConfig.blackKnight = Blue;
    blueEssenceConfig.blackBishop = Blue;
    blueEssenceConfig.blackRook = Blue;

    SimulationManager::simulateGames(gameCount, blueEssenceConfig, malakhDepth, fairyStockfishDepth);
    */
}

void updateSimulationScores()
{
    DatabaseManager::initConnectionString();
    DatabaseConnection conn;

    conn.updateScores();
}

int main()
{
    ZobristHashing::init();

    // testEngine();
    // testUCI();
    // testEnsemble();

    startUCI();
    // startSimulation();
    // updateSimulationScores();

    return 0;
}
