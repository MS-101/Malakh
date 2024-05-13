/*****************************************************************//**
 * \file   simulation.h
 * \brief  Prostredie na simuláciu šachových hier medzi enginom Malakh a FairyStockfish.
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#pragma once

#include <board.h>


/**
 * Trieda zodpovedná za simuláciu hier medzi enginom Malakh a FairyStockfish.
 */
class SimulationManager {
public:
	/**
	 * Pomocou tejto metódy spustíme simuláciu hier medzi enginami Malakh a FairyStockfish.
	 * 
	 * \param gameCounter Počet hier, ktoré majú byť odohrané.
	 * \param essenceArgs Nastavenie esencií.
	 * \param malakhDepth Hĺbka prehľadávania enginu Malakh.
	 * \param fairyStockfishDepth Hĺbka prehľadávanie enginu FairyStockfish.
	 * \param useEnsemble Hodnota pomocou ktorej môžeme nastaviť 'basic' (false) alebo 'ensemble' (true) AI režim pre engine Malakh.
	 * \param useDB Hodnota pomocou ktorej môžeme vypnúť alebo zapnúť ukladanie podrobných výsledkov simulácie do databázy.
	 * \param outputFilename Výstupný súbor do ktorého zapíšeme výsledky simulácie.
	 */
	static void simulateGames(int gameCounter, EssenceArgs essenceArgs, int malakhDepth, int fairyStockfishDepth, bool useEnsemble, bool useDB, std::string outputFilename);
};
