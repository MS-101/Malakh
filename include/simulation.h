#pragma once

#include <board.h>


class SimulationManager {
public:
	static void simulateGames(int gameCounter, EssenceArgs essenceArgs, int malakhDepth, int fairyStockfishDepth, bool useEnsemble, bool useDB, std::string outputFilename);
};
