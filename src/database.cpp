#include "database.h"

#include <pqxx/pqxx>
#include <iostream>
#include <sstream>
#include <fstream>

std::string ConnectionString::toString()
{
    std::string value = "dbname = " + dbname;
    value += " user = " + user;
    value += " password = " + password;
    value += " hostaddr = " + hostaddr;
    value += " port = " + port;

    return value;
}

ConnectionString DatabaseManager::connectionString;

void DatabaseManager::initConnectionString()
{
    loadEnvFromFile(".env");

    connectionString.dbname = std::getenv("DB_NAME");
    connectionString.user = std::getenv("DB_USER");
    connectionString.password = std::getenv("DB_PASSWORD");
    connectionString.hostaddr = std::getenv("DB_HOST");
    connectionString.port = std::getenv("DB_PORT");
}

void DatabaseManager::loadEnvFromFile(std::string filePath)
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

DatabaseConnection::DatabaseConnection() : connection(DatabaseManager::connectionString.toString()), txn(connection)
{
}

void DatabaseConnection::addBoardResult(unsigned long long boardHash, GameResult gameResult)
{
    try {
        pqxx::result result;

        switch (gameResult) {
        case white:
            result = txn.exec_params(R"(
                INSERT INTO public.boards_results (board_hash, white_count)
                VALUES ($1, 1)
                ON CONFLICT (board_hash)
                DO UPDATE SET white_count = boards_results.white_count + 1;
            )", boardHash);
            break;
        case black:
            result = txn.exec_params(R"(
                INSERT INTO public.boards_results (board_hash, black_count)
                VALUES ($1, 1)
                ON CONFLICT (board_hash)
                DO UPDATE SET black_count = boards_results.black_count + 1;
            )", boardHash);
            break;
        case stalemate:
            result = txn.exec_params(R"(
                INSERT INTO public.boards_results (board_hash, stalemate_count)
                VALUES ($1, 1)
                ON CONFLICT (board_hash)
                DO UPDATE SET stalemate_count = boards_results.stalemate_count + 1;
            )", boardHash);
            break;
        }

        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void DatabaseConnection::addModelTraining(int idModel, unsigned long long boardHash, GameResult gameResult)
{
    try {
        pqxx::result result;

        switch (gameResult) {
        case white:
            result = txn.exec_params(R"(
                INSERT INTO public.models_training (id_model, board_hash, white_count)
                VALUES ($1, $2, 1)
                ON CONFLICT (id_model, board_hash)
                DO UPDATE SET white_count = models_training.white_count + 1;
            )", idModel, boardHash);
            break;
        case black:
            result = txn.exec_params(R"(
                INSERT INTO public.models_training (id_model, board_hash, black_count)
                VALUES ($1, $2, 1)
                ON CONFLICT (id_model, board_hash)
                DO UPDATE SET black_count = models_training.black_count + 1;
            )", idModel, boardHash);
            break;
        case stalemate:
            result = txn.exec_params(R"(
                INSERT INTO public.models_training (id_model, board_hash, stalemate_count)
                VALUES ($1, $2, 1)
                ON CONFLICT (id_model, board_hash)
                DO UPDATE SET stalemate_count = models_training.stalemate_count + 1;
            )", idModel, boardHash);
            break;
        }

        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
