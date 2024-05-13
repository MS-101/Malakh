#pragma once

/*****************************************************************//**
 * \file   database.h
 * \brief  Spojenie s databázou PostgreSQL pre ukladanie výsledkov simulácie.
 *
 * \author Martin Šváb
 * \date   Máj 2024
 *********************************************************************/

#define NOMINMAX
#include <board.h>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <pqxx/transaction.hxx>


/**
 * Trieda potrebná na pripojenie do databázy.
 */
struct ConnectionString {
	/*
	* Meno databázy
	*/
	std::string dbname;

	/**
	 * Meno používateľa.
	 */
	std::string user;

	/**
	 * Heslo používateľa.
	 */
	std::string password;

	/**
	 * Adresa hostiteľa databázy.
	 */
	std::string hostaddr;

	/**
	 * Port databázy.
	 */
	std::string port;

	/**
	 * Z atribútov je vygenerovaný konekčný string.
	 * 
	 * \return Konekčný string.
	 */
	std::string toString();
};

/**
 * Trieda ktorá pracuje s konekčným stringom a jej načítaním.
 */
class DatabaseManager {
public:
	/**
	 * Aktuálny konekčný string.
	 */
	static ConnectionString connectionString;

	/**
	 * Inicializácia konekčného stringu z premenných prostredia.
	 */
	static void initConnectionString();
private:
	/**
	 * Načítanie premenných prostredia zo súboru.
	 * 
	 * \param filePath Názov súboru kde sú uložené premenné prostredia.
	 */
	static void loadEnvFromFile(std::string filePath);
};

/**
 * Konkrétne pripojenie na databáze.
 */
class DatabaseConnection {
public:
	/**
	 * Konštruktor vytvorí spojenie pomocou konekčného stringu z triedy DatabaseManager.
	 */
	DatabaseConnection();

	/**
	 * Pre danú konfiguráciu esencií zistíme jej ID.
	 * 
	 * \param essenceArgs Konfigurácia esencií.
	 * \return ID konfigurácie esencií.
	 */
	int getIdEssenceConfig(EssenceArgs essenceArgs);

	/**
	 * Pomocou tejto metódy môžeme upraviť skóre šachových pozícii uložených v databáze pri zmene evaluačnej funkcie.
	 */
	void updateScores();

	/**
	 * Do databázy uložíme výsledok hry pre danú šachovú pozíciu.
	 * Ak táto pozícia ešte v databáze neexistuje, tak vložíme nový záznam.
	 * Ak táto pozícia sa už v databáze nachádza, tak len inkrementujeme počítadlo výsledku.
	 * 
	 * \param board Uložená šachovnica.
	 * \param idEssenceConfig ID konfigurácie esencií.
	 * \param gameResult Výsledok hry.
	 */
	void addBoardResult(Board board, int idEssenceConfig, GameResult gameResult);
private:
	/**
	 * Pripojenie do databázy.
	 */
	pqxx::connection connection;

	/**
	 * Tranzakcia v databáze.
	 */
	pqxx::work txn;
};
