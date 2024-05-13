#pragma once

/*****************************************************************//**
 * \file   bitboard.h
 * \brief  Definícia bitboardovej reprezentácie šachovnice.
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/


/**
 * Táto štruktúra reprezentuje určitú vlastnosť šachovnice pomocou 64-bitového čísla. Každý bit reprezentuje určitú pozíciu na šachovnici.
 */
struct BitBoard {
public:
	/**
	 * 64-bitové číslo reprezentujúce určitú vlastnosť na šachovnici.
	 */
	unsigned long long value = 0;

	/**
	* Pre požadovanú pozíciu na šachovnici táto funkcia vráti jej bit.
	* 
	* \param x Koordinát X požadovanej pozície.
	* \param y Koordinát y požadovanej pozície.
	*/
	unsigned long long getBit(char x, char y);

	/**
	* Pre požadovanú pozíciu na šachovnici táto funkcia nastaví jej bit na hodnotu 1.
	* 
	* \param x Koordinát X požadovanej pozície.
	* \param y Koordinát y požadovanej pozície.
	*/
	void setBit(char x, char y);

	/**
	* Pre požadovanú pozíciu na šachovnici táto funkcia nastaví jej bit na hodnotu 0.
	* 
	* \param x Koordinát X požadovanej pozície.
	* \param y Koordinát y požadovanej pozície.
	*/
	void clearBit(char x, char y);

	/**
	* Táto funkcia vypíše jednotlivé bity bitboardu.
	* 
	* \param x Koordinát X požadovanej pozície.
	* \param y Koordinát y požadovanej pozície.
	*/
	void printBits();

	/**
	* Táto funkcia zistí či požadovaná pozícia je v blízkosti bitu 1.
	* 
	* \param x Koordinát X požadovanej pozície.
	* \param y Koordinát y požadovanej pozície.
	*/
	bool getKingAttack(char x, char y);

	/**
	* Táto funkcia vypočíta vzdialenosť medzi požadovanou pozíciou na šachovnici a najbližším bitom 1 na bitboarde.
	* 
	* \param x Koordinát X požadovanej pozície.
	* \param y Koordinát y požadovanej pozície.
	*/
	char getTropism(char x, char y);
private:

	/**
	* Táto funkcia vytvorí masku potrebnú na získanie bitu pre požadovanú pozíciu na šachovnici.
	* 
	* \param x Koordinát X požadovanej pozície.
	* \param y Koordinát y požadovanej pozície.
	*/
	unsigned long long getMask(char x, char y);

	/**
	* Táto funkcia vytvorí masku s paternou kráľa - všetky bity 1 sú rozšírené o ich štvorcové okolie.
	* 
	* \param value Hodnota bitboardu z ktorého generujeme paternu.
	*/
	unsigned long long getKingPattern(unsigned long long value);

	/**
	* Táto funkcia vytvorí masku s paternou kráľa - všetky bity 1 sú rozšírené o ich krížové okolie.
	* 
	* \param value Hodnota bitboardu z ktorého generujeme paternu.
	*/
	unsigned long long getTropismPattern(unsigned long long value);
};