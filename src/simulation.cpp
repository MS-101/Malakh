#include "simulation.h"
#include "fairyStockfish.h"
#include "search.h"
#include <iostream>


void SimulationManager::simulateGames(int gameCounter, EssenceArgs essenceArgs, int malakhDepth, int fairyStockfishDepth)
{
	Board board;

	FairyStockfish fairyStockfish;
	fairyStockfish.start();

	fairyStockfish.sendCommand("uci");
	fairyStockfish.waitForResponse("uciok");

	fairyStockfish.sendCommand("isready");
	fairyStockfish.waitForResponse("readyok");

	for (int i = 0; i < gameCounter; i++)
	{
		board.initBoard(essenceArgs);

		fairyStockfish.sendCommand("ucinewgame");

		bool malakhTurn = false;
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

			if (!board.makeMove(bestMove))
			{
				int foo = 0;
			}
			fairyStockfish.sendCommand("position " + board.toString() + " moves " + bestMove.toString());
			
			malakhTurn = !malakhTurn;
		}

		int foo = 0;
		// iterate through moves and store result in database
	}

	fairyStockfish.exit();
}
