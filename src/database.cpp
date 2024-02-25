#include "database.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Windows.h"
#include <pqxx/pqxx>


std::unordered_map<unsigned long long, TrainingData> DatabaseManager::cache;
ConnectionString DatabaseManager::connectionString = {};

void DatabaseManager::init()
{
    loadEnvFromFile(".env");
    
    connectionString.dbname = std::getenv("DB_NAME");
    connectionString.user = std::getenv("DB_USER");
    connectionString.password = std::getenv("DB_PASSWORD");
    connectionString.hostaddr = std::getenv("DB_HOST");
    connectionString.port = std::getenv("DB_PORT");

    loadTrainingData();
}

void DatabaseManager::loadEnvFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream lineStream(line);
            std::string key, value;
            if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
                _putenv_s(key.c_str(), value.c_str());
            }
        }

        file.close();
    }
}

void DatabaseManager::loadTrainingData()
{
    try {
        pqxx::connection connection(connectionString.toString());
        pqxx::work txn(connection);
        pqxx::result result = txn.exec_params(R"(
            SELECT "boardHash", "whiteCount", "blackCount", "stalemateCount"
            FROM public."trainingData"
        )");

        for (const auto& row : result) {
            unsigned long long boardHash = row[R"("boardHash")"].as<unsigned long long>();

            TrainingData data{};
            data.whiteCount = row[R"("whiteCount")"].as<int>();
            data.blackCount = row[R"("blackCount")"].as<int>();
            data.stalemateCount = row[R"("stalemateCount")"].as<int>();

            cache[boardHash] = data;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int DatabaseManager::getTrainingValue(unsigned long long boardHash)
{
    TrainingData data = cache[boardHash];
    return (data.whiteCount - (data.blackCount + data.stalemateCount)) * data.getTotalCount();
}

void DatabaseManager::saveWhiteVictory(unsigned long long boardHash)
{
}

void DatabaseManager::saveBlackVictory(unsigned long long boardHash)
{
}

void DatabaseManager::saveStalemate(unsigned long long boardHash)
{
}

std::string ConnectionString::toString()
{
    std::string value = "dbname = " + dbname;
    value += " user = " + user;
    value += " password = " + password;
    value += " hostaddr = " + hostaddr;
    value += " port = " + port;

    return value;
}

int TrainingData::getTotalCount()
{
    return whiteCount + blackCount + stalemateCount;
}
