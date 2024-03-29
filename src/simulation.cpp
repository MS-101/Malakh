#include <simulation.h>
#include <fairyStockfish.h>
#include <searching.h>
#include <random.h>
#include <database.h>
#include <iostream>


void SimulationManager::simulateGames(int gameCounter, EssenceArgs essenceArgs, int malakhDepth, int fairyStockfishDepth)
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
	for (int i = 0; i < gameCounter; i++)
	{
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

		fairyStockfish.sendCommand("ucinewgame");
		fairyStockfish.sendCommand("position fen " + board.toString());

		bool malakhTurn = Random::coinFlip();
		std::vector<unsigned long long> gameHistory{};
		gameHistory.push_back(board.hash.value);

		GameResult result;
		while ((result = board.getResult()) == Unresolved)
		{
			LegalMove bestMove;

			if (malakhTurn)
			{
				auto result = SearchManager::calculateBestMove_threads(board, malakhDepth, 4, false);
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

			board.makeMove(bestMove);
			gameHistory.push_back(board.hash.value);
			std::string command = "position fen " + board.toString() + " moves " + bestMove.toString();
			fairyStockfish.sendCommand(command);

			if (malakhTurn) {
				std::cout << "Malakh: " << bestMove.toString() << std::endl;
			} else {
				std::cout << "FairyStockfish: " << bestMove.toString() << std::endl;
			}
			
			malakhTurn = !malakhTurn;
		}

		for (unsigned long long boardHash : gameHistory) {
			conn.addBoardResult(boardHash, idEssenceConfig, result);
		}
	}

	fairyStockfish.exit();
}
