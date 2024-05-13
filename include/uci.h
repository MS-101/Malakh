#pragma once

/*****************************************************************//**
 * \file   uci.h
 * \brief  UCI protokol umožňuje komunikáciu medzi našim enginom a GUI alebo inými enginami.
 *
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#include <searching.h>
#include <string>


/**
 * Trieda zodpovedná za UCI režim aplikácie.
 */
class uci {
public:
	/**
	 * Touto metódou spustíme UCI režim kde aplikácia čaká na UCI príkazy.
	 */
	void run();

	/**
	 * Táto metóda spracuváva UCI príkazy.
	 * 
	 * \param command Prijatý príkaz
	 * \return Ak prijatý príkaz terminuje aplikáciu tak 1, v opačnom prípade 0.
	 */
	bool parseCommand(std::string command);
private:
	/**
	 * Konfigurácia esencií aktuálnej hry.
	 */
	EssenceArgs essenceConfig;

	/**
	 * Stav šachovnice aktuálnej hry.
	 */
	Board board;

	/**
	 * Nastavenie AI režimu. Ak je hodnota false, tak používame 'basic' režim. Ak je hodnota true, tak používame 'ensemble' režim.
	 */
	bool useEnsemble = false;
};