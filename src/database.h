#pragma once

#include "board.h"
#include <pqxx/pqxx>
#include <string>

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
	void addBoardResult(unsigned long long boardHash, GameResult gameResult);
	void addModelTraining(int idModel, unsigned long long boardHash, GameResult gameResult);
private:
	pqxx::connection connection;
	pqxx::work txn;
};
