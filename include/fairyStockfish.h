#pragma once

/*****************************************************************//**
 * \file   fairyStockfish.h
 * \brief  Ovládač pre engine FairyStockfish.
 * 
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>
#include <board.h>


/**
 * Pomocou tejto triedy ovládame engine FairyStockfish.
 */
class FairyStockfish {
public:
	/**
	 * Engine spustíme s danou konfiguráciou esencií.
	 * 
	 * \param essenceArgs Konfigurácia esencií.
	 * \return Vráti hodnotu true ak bol engine úspešne spustený, v opačnom prípade vráti false.
	 */
	bool start(EssenceArgs essenceArgs);

	/**
	 * Touto metódou spustený engine terminujeme.
	 */
	void exit();

	/**
	 * Enginu pošleme UCI príkaz.
	 * 
	 * \param command UCI príkaz.
	 * \return Ak bol príkaz úspešne odoslaný tak vráti true, v opačnom prípade vráti false.
	 */
	bool sendCommand(std::string command);

	/**
	 * Vytvorenie konfiguračného súboru pre danú konfiguráciu esencií.
	 * 
	 * \param essenceArgs Konfigurácia esencií.
	 */
	void setConfigFile(EssenceArgs essenceArgs);

	/**
	 * Od enginu prijímame odpovede až kým nenájdeme očakávanú odpoveď.
	 * 
	 * \param target Začiatok odpovede na ktorú čakáme. (napr. bestmove)
	 * \return Prijatá správa.
	 */
	std::string waitForResponse(std::string target);

	/**
	 * Od enginu prijmeme odpoveď.
	 * 
	 * \return Prijatá správa.
	 */
	std::string receiveResponse();
private:
	PROCESS_INFORMATION piProcInfo{};
	HANDLE handleStdinRead = NULL;
	HANDLE handleStdinWrite = NULL;
	HANDLE handleStdoutRead = NULL;
	HANDLE handleStdoutWrite = NULL;
};
