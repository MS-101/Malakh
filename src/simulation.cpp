#include <simulation.h>
#include <fairyStockfish.h>
#include <searching.h>
#include <random.h>
#include <database.h>
#include <iostream>
#include <fstream>


void SimulationManager::simulateGames(int gameCounter, EssenceArgs essenceArgs, int malakhDepth, int fairyStockfishDepth, std::string outpufFilename)
{
	Random::initSeed();

	DatabaseManager::initConnectionString();
	DatabaseConnection conn;
	int idEssenceConfig = conn.getIdEssenceConfig(essenceArgs);

	FairyStockfish fairyStockfish;
	fairyStockfish.start(essenceArgs);

	fairyStockfish.sendCommand("uci");
	fairyStockfish.waitForResponse("uciok");

	fairyStockfish.sendCommand("isready");
	fairyStockfish.waitForResponse("readyok");

	Board board;
	int malakhVictories = 0;
	int errorCount = 0;

	for (int i = 0; i < gameCounter + errorCount; i++)
	{
		std::cout << "Game simulation " << (i+1) << ":" << std::endl << std::endl;

		while (true)
		{
			int turnCount = Random::generateRandomNumber(6, 12);

			board.initBoard(essenceArgs);
			for (int i = 0; i < turnCount; i++)
			{
				auto legalMoves = board.getLegalMoves();
				if (legalMoves.empty())
					continue;

				board.makeMove(Random::getRandomElement<LegalMove>(legalMoves));
			}

			if (board.getResult() == Unresolved)
				break;
		}

		std::cout << "starting position: " << board.toString() << std::endl;
		// board.printBoard();

		fairyStockfish.sendCommand("ucinewgame");
		fairyStockfish.sendCommand("position fen " + board.toString());

		bool malakhWhite = Random::coinFlip();
		bool malakhTurn = malakhWhite;
		std::vector<Board> gameHistory{};
		gameHistory.push_back(board);

		int lazyCounter = 0;
		bool error = false;

		GameResult result;
		while ((result = board.getResult()) == Unresolved)
		{
			LegalMove bestMove;

			if (malakhTurn)
			{
				auto result = SearchManager::calculateBestMove_threads(board, malakhDepth, 4, false, false);
				if (result.first)
					bestMove = result.second;
			}
			else
			{
				fairyStockfish.sendCommand("go depth " + std::to_string(fairyStockfishDepth));
				std::string response = fairyStockfish.waitForResponse("bestmove");

				std::stringstream responseSS(response);
				std::vector<std::string> tokens;
				std::string token;

				while (std::getline(responseSS, token, ' ')) {
					tokens.push_back(token);
				}

				bestMove = LegalMove(tokens[1]);
			}

			if (malakhTurn) {
				std::cout << "Malakh: " << bestMove.toString() << std::endl;
			} else {
				std::cout << "FairyStockfish: " << bestMove.toString() << std::endl;
			}

			board.makeMove(bestMove);
			// board.printBoard();
			lazyCounter++;

			gameHistory.push_back(board);
			fairyStockfish.sendCommand("position fen " + board.toString());
			
			malakhTurn = !malakhTurn;

			if (lazyCounter >= 100) {
				error = true;
				break;
			}
		}

		if (!error)
		{
			if (result == WhiteWin && malakhWhite || result == BlackWin && !malakhWhite) {
				std::cout << "Malakh wins!" << std::endl;
				malakhVictories++;
			}
			else if (result == WhiteWin && !malakhWhite || result == BlackWin && malakhWhite) {
				std::cout << "FairyStockfish wins!" << std::endl;
			}
			else {
				std::cout << "Stalemate!" << std::endl;
			}
			std::cout << std::endl;

			for (Board board : gameHistory) {
				conn.addBoardResult(board, idEssenceConfig, result);
			}
		}
		else
		{
			errorCount++;
			std::cout << "Error!" << std::endl;
		}

		gameHistory.clear();
	}

	std::ofstream outputFile(outpufFilename);
	outputFile << "Malakh won " << malakhVictories << "/" << gameCounter << " games!" << std::endl;
	outputFile << "Errors: " << errorCount << std::endl;
	outputFile.close();

	fairyStockfish.exit();
}
