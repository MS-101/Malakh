#pragma once

/*****************************************************************//**
 * \file   ensemble.h
 * \brief  Implementácia súborového učenia.
 *
 * \author marti
 * \date   May 2024
 *********************************************************************/

#include <torch/script.h>


/**
 * Trieda zodpovedná za súborové učenie.
 */
class Ensemble {
public:
	/**
	 * Konštruktor súborového učenia načíta modely.
	 */
	Ensemble();

	/**
	 * Pomocou tejto metódy vykonáme predikciu skóre šachovej pozície pomocou vstupnej matice 8x8x2.
	 * 
	 * \param inputArray Vstupná matica šachovnica 8x8x2. Prvý kanál reprezentuje šachové figúrky. Druhý kanál reprezentuje hráča na rade.
	 * \param essenceCounts Počty esencií v hernej konfigurácii.
	 * \return Predikcia evaluácie šachovej pozície.
	 */
	double forward(int* inputArray, int* essenceCounts);
private:
	/**
	 * Modely, ktoré tvoria naše súborové učenie.
	 */
	torch::jit::script::Module models[3];
};
