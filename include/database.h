#pragma once

#define NOMINMAX
#include <board.h>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <pqxx/transaction.hxx>


struct ConnectionString {
	std::string dbname;
	std::string user;
	std::string password;
	std::string hostaddr;
	std::string port;
	std::string toString();
};

class DatabaseManager {
public:
	static ConnectionString connectionString;
	static void initConnectionString();
private:
	static void loadEnvFromFile(std::string filePath);
};

class DatabaseConnection {
public:
	DatabaseConnection();
	int getIdEssenceConfig(EssenceArgs essenceArgs);
	void updateScores();
	void addBoardResult(Board board, int idEssenceConfig, GameResult gameResult);
private:
	pqxx::connection connection;
	pqxx::work txn;
};
