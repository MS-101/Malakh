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

    auto result = SearchManager::calculateBestMove_threads(board, 4, 4, false, false);
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

    api.parseCommand("go depth 4");
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

    double value = ensemble.forward(board.inputArray, board.essenceCounts);

    std::cout << value << std::endl;
}

void startUCI()
{
    uci api;
    api.run();
}

void startSimulation()
{
    int gameCount = 10;
    int malakhDepth = 3;
    int fairyStockfishDepth = 6;
    bool useEnsemble = true;
    bool useDB = false;

    EssenceArgs redEssenceConfig;
    redEssenceConfig.whitePawn = Red;
    redEssenceConfig.whiteKnight = Red;
    redEssenceConfig.whiteBishop = Red;
    redEssenceConfig.whiteRook = Red;
    redEssenceConfig.blackPawn = Red;
    redEssenceConfig.blackKnight = Red;
    redEssenceConfig.blackBishop = Red;
    redEssenceConfig.blackRook = Red;

    SimulationManager::simulateGames(
        gameCount, redEssenceConfig, malakhDepth, fairyStockfishDepth, useEnsemble, useDB, "simulation-red.txt"
    );

    EssenceArgs blueEssenceConfig;
    blueEssenceConfig.whitePawn = Blue;
    blueEssenceConfig.whiteKnight = Blue;
    blueEssenceConfig.whiteBishop = Blue;
    blueEssenceConfig.whiteRook = Blue;
    blueEssenceConfig.blackPawn = Blue;
    blueEssenceConfig.blackKnight = Blue;
    blueEssenceConfig.blackBishop = Blue;
    blueEssenceConfig.blackRook = Blue;
    
    SimulationManager::simulateGames(
        gameCount, blueEssenceConfig, malakhDepth, fairyStockfishDepth, useEnsemble, useDB, "simulation-blue.txt"
    );

    EssenceArgs mixedEssenceConfig;
    mixedEssenceConfig.whitePawn = Red;
    mixedEssenceConfig.whiteKnight = Red;
    mixedEssenceConfig.whiteBishop = Blue;
    mixedEssenceConfig.whiteRook = Blue;
    mixedEssenceConfig.blackPawn = Red;
    mixedEssenceConfig.blackKnight = Red;
    mixedEssenceConfig.blackBishop = Blue;
    mixedEssenceConfig.blackRook = Blue;

    SimulationManager::simulateGames(
        gameCount, mixedEssenceConfig, malakhDepth, fairyStockfishDepth, useEnsemble, useDB, "simulation-mixed.txt"
    );
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
