#include <database.h>
#include <fstream>
#include <cmath>


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

int DatabaseConnection::getIdEssenceConfig(EssenceArgs essenceArgs)
{
    int retValue = 0;

    // create this configuration if it does not exist
    txn.exec_params(R"(
        INSERT INTO public.essence_configs 
        (white_pawn, white_rook, white_knight, white_bishop,
         black_pawn, black_rook, black_knight, black_bishop)
        VALUES
        ($1, $2, $3, $4, $5, $6, $7, $8)
        ON CONFLICT
        (white_pawn, white_rook, white_knight, white_bishop,
         black_pawn, black_rook, black_knight, black_bishop)
        DO NOTHING
    )", (int)essenceArgs.whitePawn, (int)essenceArgs.whiteRook, (int)essenceArgs.whiteKnight, (int)essenceArgs.whiteBishop,
        (int)essenceArgs.blackPawn, (int)essenceArgs.blackRook, (int)essenceArgs.blackKnight, (int)essenceArgs.blackBishop);
    txn.commit();

    // retrieve id of this configuration
    pqxx::result result = txn.exec_params(R"(
        SELECT id_essence_config
        FROM public.essence_configs
        WHERE white_pawn = $1 AND white_rook = $2 AND white_knight = $3 AND white_bishop = $4
        AND black_pawn = $5 AND black_rook = $6 AND black_knight = $7 AND black_bishop = $8
    )", (int)essenceArgs.whitePawn, (int)essenceArgs.whiteRook, (int)essenceArgs.whiteKnight, (int)essenceArgs.whiteBishop,
        (int)essenceArgs.blackPawn, (int)essenceArgs.blackRook, (int)essenceArgs.blackKnight, (int)essenceArgs.blackBishop);

    if (!result.empty()) {
        retValue = result[0]["id_essence_config"].as<int>();
    }

    return retValue;
}

void DatabaseConnection::addBoardResult(Board board, int idEssenceConfig, GameResult gameResult)
{
    // insert input features of board if they do not exist
    txn.exec_params(R"(
        INSERT INTO public.boards
        (board_hash, white_pawns, white_rooks, white_knights, white_bishops, white_queens, white_kings,
        black_pawns, black_rooks, black_knights, black_bishops, black_queens, black_kings, turn)
        VALUES
        ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14)
        ON CONFLICT (board_hash)
        DO NOTHING;
    )", (long long)board.hash.value,
        (long long)board.pieces[getPieceIndex(White, Pawn)].value,
        (long long)board.pieces[getPieceIndex(White, Rook)].value,
        (long long)board.pieces[getPieceIndex(White, Knight)].value,
        (long long)board.pieces[getPieceIndex(White, Bishop)].value,
        (long long)board.pieces[getPieceIndex(White, Queen)].value,
        (long long)board.pieces[getPieceIndex(White, King)].value,
        (long long)board.pieces[getPieceIndex(Black, Pawn)].value,
        (long long)board.pieces[getPieceIndex(Black, Rook)].value,
        (long long)board.pieces[getPieceIndex(Black, Knight)].value,
        (long long)board.pieces[getPieceIndex(Black, Bishop)].value,
        (long long)board.pieces[getPieceIndex(Black, Queen)].value,
        (long long)board.pieces[getPieceIndex(Black, King)].value,
        (int)board.curTurn
    );
    txn.commit();

    // update target values of board
    switch (gameResult) {
    case WhiteWin:
        txn.exec_params(R"(
            INSERT INTO public.board_results
            (board_hash, id_essence_config, white_count)
            VALUES
            ($1, $2, 1)
            ON CONFLICT (board_hash, id_essence_config)
            DO UPDATE SET white_count = board_results.white_count + 1
        )", (long long)board.hash.value, idEssenceConfig);
        break;
    case BlackWin:
        txn.exec_params(R"(
            INSERT INTO public.board_results
            (board_hash, id_essence_config, black_count)
            VALUES
            ($1, $2, 1)
            ON CONFLICT (board_hash, id_essence_config)
            DO UPDATE SET black_count = board_results.black_count + 1;
        )", (long long)board.hash.value, idEssenceConfig);
        break;
    case Stalemate:
        txn.exec_params(R"(
            INSERT INTO public.board_results
            (board_hash, id_essence_config, stalemate_count)
            VALUES
            ($1, $2, 1)
            ON CONFLICT (board_hash, id_essence_config)
            DO UPDATE SET stalemate_count = board_results.stalemate_count + 1;
        )", (long long)board.hash.value, idEssenceConfig);
        break;
    }
    txn.commit();
}

std::vector<TrainingData> DatabaseConnection::getTrainingData(int idEssenceConfig, int page, int pageSize)
{
    std::vector<TrainingData> retValue;

    int limit = pageSize;
    int offset = (page - 1) * pageSize;

     pqxx::result result = txn.exec_params(R"(
        SELECT board_hash, white_count, black_count, stalemate_count
        FROM public.boards_results
        WHERE id_essence_config = $1
        ORDER BY board_hash ASC
        LIMIT $2 OFFSET $3
    )", idEssenceConfig, limit, offset);
    
    for (const auto& row : result) {
        unsigned long long boardHash = row["board_hash"].as<unsigned long long>();
        int whiteCount = row["white_count"].as<int>();
        int blackCount = row["black_count"].as<int>();
        int stalemateCount = row["stalemate_count"].as<int>();

        TrainingData trainingData;
        trainingData.boardHash = boardHash;
        trainingData.value = log2(whiteCount - (blackCount + 0.5 * stalemateCount));

        retValue.push_back(trainingData);
    }

    return retValue; 
}
