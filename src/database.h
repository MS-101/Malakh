#pragma once

#include <unordered_map>
#include <string>

struct ConnectionString {
	std::string dbname;
	std::string user;
	std::string password;
	std::string hostaddr;
	std::string port;
	std::string toString();
};

struct TrainingData {
	int whiteCount = 0;
	int blackCount = 0;
	int stalemateCount = 0;
	int getTotalCount();
};

class DatabaseManager {
public:
	static void init();
	static int getTrainingValue(unsigned long long boardHash);
	static void saveWhiteVictory(unsigned long long boardHash);
	static void saveBlackVictory(unsigned long long boardHash);
	static void saveStalemate(unsigned long long boardHash);
private:
	static std::unordered_map<unsigned long long, TrainingData> cache;
	static struct ConnectionString connectionString;
	static void loadEnvFromFile(const std::string& filePath);
	static void loadTrainingData();
};