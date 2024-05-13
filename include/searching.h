#pragma once

/*****************************************************************//**
 * \file   searching.h
 * \brief  Prehľadávanie šachovnice.
 *
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#include <board.h>
#include <transpositions.h>
#include <chrono>
#include <ensemble.h>


/**
 * Argumenty prehľadávacej funkcie.
 */
struct SearchArgs {
	/**
	 * Aktuálna hĺbka prehľadávania.
	 */
	int curDepth = 0;

	/**
	 * Maximálna hĺbka prehľadávania.
	 */
	int maxDepth = 0;

	/**
	 * Alpha hodnota v pruning algoritme.
	 */
	int alpha = INT_MIN;

	/**
	 * Beta hodnota v pruning algoritme.
	 */
	int beta = INT_MAX;
};

/**
 * Výkonnostné argumenty prehľadávacej funkcie.
 */
struct PerformanceArgs {
	/**
	 * Počiatočná časová značka.
	 */
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	/**
	 * Počet pozícií, ktoré boli aktuálne prehľadané.
	 */
	int positionsCur = 0;

	/**
	 * Počet všetkých pozícii, ktoré boli prehľadané.
	 */
	int positionsTotal = 0;

	/**
	 * Celkový čas strávený prehľadávaním pozícií.
	 */
	long long durationTotal = 0;

	/**
	 * Mutex na uzamknutie výkonnostných argumentov.
	 */
	static std::mutex mutex;

	/**
	 * Vypíše aktuálny výkon prehľadávacej funkcie.
	 */
	void printPerformance();

	/**
	 * Vypíše aktuálny výkon prehľadávacej funkcie.
	 * 
	 * \param stop Konečná časová značka.
	 * \param durationCur Čas výpočtu.
	 */
	void printPerformance(std::chrono::high_resolution_clock::time_point stop, long long durationCur);
};

/**
 * Trieda zodpovedná za prehľadávanie pozícií na šachovnici.
 */
class SearchManager {
public:
	/**
	 * Cache s transpozíciami.
	 */
	static TranspositionCache cache;

	/**
	 * Súbor modelov, ktorý sa používa v 'ensemble' AI na evaluáciu.
	 */
	static Ensemble ensemble;

	/**
	 * Sériový výpočet najlepšieho pohybu.
	 * 
	 * \param board Šachovnica kde hľadáme najlepší pohyb.
	 * \param depth Hĺbka do ktorej vykonávame prehľadávanie.
	 * \param useEnsemble Bit ktorým môžeme nastaviť či chceme namiesto štandardnej evaluačnej funkcie použiť súborové učenie.
	 * \param debug Bit ktorým môžeme nastaviť či chceme vypisovať debug správy.
	 * \return Vráti pár bitu, ktorý reprezentuje či sme našli legálny pohyb a najlepšieho možného pohybu.
	 */
	static std::pair<bool, LegalMove> calculateBestMove(Board board, int depth, bool useEnsemble, bool debug);

	/**
	 * Paralelný výpočet najlepšieho pohybu.
	 * 
	 * \param board Šachovnica kde hľadáme najlepší pohyb.
	 * \param depth Hĺbka do ktorej vykonávame prehľadávanie.
	 * \param threadCount Počet paralelných vlákien, ktoré hľadajú najlepší pohyb.
	 * \param useEnsemble Bit ktorým môžeme nastaviť či chceme namiesto štandardnej evaluačnej funkcie použiť súborové učenie.
	 * \param debug Bit ktorým môžeme nastaviť či chceme vypisovať debug správy.
	 * \return Vráti pár bitu, ktorý reprezentuje či sme našli legálny pohyb a najlepšieho možného pohybu.
	 */
	static std::pair<bool, LegalMove> calculateBestMove_threads(Board board, int depth, int threadCount, bool useEnsemble, bool debug);

	/**
	 * Rekurzívny algoritmus, ktorý vypočíta hodnotu najlepšieho možného pohybu.
	 * 
	 * \param board Šachovnica kde hľadáme najlepší pohyb.
	 * \param playerColor Farba maximalizujúceho hráča.
	 * \param searchArgs Argumenty prehľadávania.
	 * \param performanceArgs Výkonnostné argumenty.
	 * \param useEnsemble Bit ktorým môžeme nastaviť či chceme namiesto štandardnej evaluačnej funkcie použiť súborové učenie.
	 * \param debug Bit ktorým môžeme nastaviť či chceme vypisovať debug správy.
	 * \return Hodnota najlepšieho pohybu.
	 */
	static int minimax(Board board, PieceColor playerColor, SearchArgs searchArgs, PerformanceArgs* performanceArgs, bool useEnsemble, bool debug);
private:
	/**
	 * Limit Q-searchu (zabraňuje horizont efekt).
	 */
	static const int qLimit = 0;
};
