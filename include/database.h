#pragma once

#include <board.h>
#include <pqxx/pqxx>
#include <string>
#include <vector>


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

struct TrainingData {
	unsigned long long boardHash;
	double value;
};

class DatabaseConnection {
public:
	DatabaseConnection();
	int getIdEssenceConfig(EssenceArgs essenceArgs);
	void addBoardResult(Board board, int idEssenceConfig, GameResult gameResult);
	std::vector<TrainingData> getTrainingData(int idEssenceConfig, int page, int pageSize);
private:
	pqxx::connection connection;
	pqxx::work txn;
};
