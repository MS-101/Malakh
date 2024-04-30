#include <uci.h>
#include <simulation.h>
#include <nn.h>
#include <iostream>


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
    api.parseCommand("setoption name WhitePawn value Red");
    api.parseCommand("setoption name WhiteKnight value Red");
    api.parseCommand("setoption name WhiteBishop value Red");
    api.parseCommand("setoption name WhiteRook value Red");
    api.parseCommand("setoption name BlackPawn value Red");
    api.parseCommand("setoption name BlackKnight value Red");
    api.parseCommand("setoption name BlackBishop value Red");
    api.parseCommand("setoption name BlackRook value Red");
    api.parseCommand("ucinewgame");

    // first turn
    api.parseCommand("position curpos moves e2e4");
    api.parseCommand("position curpos moves d7c6");

    // 2nd turn
    api.parseCommand("position curpos moves d1f3");
    api.parseCommand("position curpos moves g8f6");

    // 3rd turn
    api.parseCommand("position curpos moves f3f4");
    api.parseCommand("position curpos moves d8d4");

    // 4th turn
    api.parseCommand("position curpos moves g1g4");
    api.parseCommand("position curpos moves f6e4");

    // 5th turn
    api.parseCommand("position curpos moves e1e2");
    api.parseCommand("position curpos moves e4c3");

    // 6th turn
    api.parseCommand("position curpos moves d2c3");
    api.parseCommand("position curpos moves c8g4");

    // 7th turn
    api.parseCommand("position curpos moves f2f3");
    api.parseCommand("position curpos moves f8e6");

    // 8th turn
    api.parseCommand("position curpos moves f4d4");
    api.parseCommand("position curpos moves g4f3");

    // 9th turn
    api.parseCommand("position curpos moves g2f3");
    api.parseCommand("position curpos moves e6d4");

    // 10th turn
    api.parseCommand("position curpos moves c3d4");
    api.parseCommand("position curpos moves b8d7");

    // 11th turn
    api.parseCommand("position curpos moves c2c3");
    api.parseCommand("position curpos moves h8g6");

    // 12th turn
    api.parseCommand("position curpos moves b1b4");
    api.parseCommand("position curpos moves g6e6");

    // 13th turn
    api.parseCommand("position curpos moves e2d3");
    api.parseCommand("position curpos moves e6e1");

    // 14th turn
    api.parseCommand("position curpos moves d3d2");
    api.parseCommand("position curpos moves e1e6");

    // 15th turn
    api.parseCommand("position curpos moves f1h3");
    api.parseCommand("position curpos moves e6d6");

    // 16th turn
    api.parseCommand("position curpos moves h3d7");
    api.parseCommand("position curpos moves e8d7");

    // 17th turn
    api.parseCommand("position curpos moves d2d3");
    api.parseCommand("position curpos moves c6c4");

    // 18th turn
    api.parseCommand("position curpos moves d3e3");
    api.parseCommand("position curpos moves a8b6");

    // 19th turn
    api.parseCommand("position curpos moves f3e4");
    api.parseCommand("position curpos moves e7e5");

    // 20th turn
    api.parseCommand("position curpos moves h1f1");
    api.parseCommand("position curpos moves e5d4");

    // 21th turn
    api.parseCommand("position curpos moves c3d4");
    api.parseCommand("position curpos moves b6b4");

    // 22th turn
    api.parseCommand("position curpos moves f1f7");
    api.parseCommand("position curpos moves d7c6");

    // 23th turn
    api.parseCommand("position curpos moves f7g7");
    api.parseCommand("position curpos moves c4c2");

    // 24th turn
    api.parseCommand("position curpos moves d4d5");
    api.parseCommand("position curpos moves c6b6");

    // 25th turn
    api.parseCommand("position curpos moves a1b3");
    api.parseCommand("position curpos moves d6e4");

    // 26th turn
    api.parseCommand("position curpos moves e3f3");
    api.parseCommand("position curpos moves c2d1q");
}

void testNN()
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
}

int main()
{
    ZobristHashing::init();

    // testEngine();
    // testUCI();
    // testNN();

    startUCI();
    // startSimulation();

    return 0;
}
