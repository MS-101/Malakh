/*****************************************************************//**
 * \file   random.h
 * \brief  Generácia náhodných čísiel
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#pragma once

#include <random>
#include <vector>


/**
 * Trieda zodpovedná za generáciu náhodných čísiel.
 */
class Random {
public:
	/**
	 * Inicializácia náhodného seedu.
	 */
	static void initSeed();

	/**
	 * Generácia náhodného čísla od 0 po 1.
	 * 
	 * \return Náhodne vygenerované číslo.
	 */
	static bool coinFlip();

	/**
	 * Generácia náhodného čísla v danom rozsahu.
	 * 
	 * \param lowerBound Spodná hranica náhodného čísla
	 * \param upperBound Vrchná hranica náhodného čísla
	 * \return Náhodne vygenerované číslo
	 */
	static int generateRandomNumber(int lowerBound, int upperBound);

	/**
	 * Výber náhodného elemetu z vektora T.
	 *
	 * \param vec Zoznam prvkov T.
	 * \return Náhodne vybraný prvok T.
	 */
	template <typename T>static T getRandomElement(const std::vector<T>& vec);
private:
	/**
	 * Generátor náhodných čísiel.
	 */
	static std::mt19937 gen;
};

template<typename T>
inline T Random::getRandomElement(const std::vector<T>& vec)
{
	if (vec.empty()) {
		return T();
	} else {
		return vec[generateRandomNumber(0, vec.size() - 1)];
	}
}
