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

		bool malakhTurn = true;
		GameResult result;

		while ((result = board.getResult()) == Unresolved)
		{
			LegalMove bestMove;

			fairyStockfish.sendCommand("go depth " + std::to_string(fairyStockfishDepth));
			std::string response = fairyStockfish.waitForResponse("bestmove");

			/*
			if (malakhTurn)
			{
				auto result = SearchManager::calculateBestMove_threads(board, malakhDepth, 4, true);
				if (result.first)
					bestMove = result.second;
			}
			else
			{
				fairyStockfish.sendCommand("go depth " + fairyStockfishDepth);
				std::string response = fairyStockfish.receiveResponse();
			}
			*/

			board.makeMove(bestMove);
			fairyStockfish.sendCommand("position " + board.toString() + " moves " + bestMove.toString());
			

			malakhTurn = !malakhTurn;
		}

		// iterate through moves and store result in database
	}

	fairyStockfish.exit();
}
