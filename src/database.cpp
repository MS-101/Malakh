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
        (white_pawns_config, white_rooks_config, white_knights_config, white_bishops_config,
         black_pawns_config, black_rooks_config, black_knights_config, black_bishops_config)
        VALUES
        ($1, $2, $3, $4, $5, $6, $7, $8)
        ON CONFLICT
        (white_pawns_config, white_rooks_config, white_knights_config, white_bishops_config,
         black_pawns_config, black_rooks_config, black_knights_config, black_bishops_config)
        DO NOTHING
    )", (int)essenceArgs.whitePawn, (int)essenceArgs.whiteRook, (int)essenceArgs.whiteKnight, (int)essenceArgs.whiteBishop,
        (int)essenceArgs.blackPawn, (int)essenceArgs.blackRook, (int)essenceArgs.blackKnight, (int)essenceArgs.blackBishop);
    txn.commit();

    // retrieve id of this configuration
    pqxx::result result = txn.exec_params(R"(
        SELECT id_essence_config
        FROM public.essence_configs
        WHERE white_pawns_config = $1 AND white_rooks_config = $2 AND white_knights_config = $3 AND white_bishops_config = $4
        AND black_pawns_config = $5 AND black_rooks_config = $6 AND black_knights_config = $7 AND black_bishops_config = $8
    )", (int)essenceArgs.whitePawn, (int)essenceArgs.whiteRook, (int)essenceArgs.whiteKnight, (int)essenceArgs.whiteBishop,
        (int)essenceArgs.blackPawn, (int)essenceArgs.blackRook, (int)essenceArgs.blackKnight, (int)essenceArgs.blackBishop);

    if (!result.empty()) {
        retValue = result[0]["id_essence_config"].as<int>();
    }

    return retValue;
}

void DatabaseConnection::updateScores()
{
    pqxx::result result = txn.exec_params(R"(
        SELECT b.board_hash
        , b.white_pawns, b.white_rooks, b.white_knights, b.white_bishops, b.white_queens, b.white_kings
        , b.black_pawns, b.black_rooks, b.black_knights, b.black_bishops, b.black_queens, b.black_kings
        , b.turn
        , e.id_essence_config
        , e.white_pawns_config, e.white_rooks_config, e.white_knights_config, e.white_bishops_config
        , e.black_pawns_config, e.black_rooks_config, e.black_knights_config, e.black_bishops_config
        FROM public.boards b
        INNER JOIN public.board_results br ON br.board_hash = b.board_hash
        INNER JOIN public.essence_configs e ON e.id_essence_config = br.id_essence_config;
    )");

    for (const auto& row : result) {
        long long boardHash = row["board_hash"].as<long long>();
        int idEssenceConfig = row["id_essence_config"].as<int>();
        int whitePawnsConfig = row["white_pawns_config"].as<int>();
        int whiteRooksConfig = row["white_rooks_config"].as<int>();
        int whiteKnightsConfig = row["white_knights_config"].as<int>();
        int whiteBishopsConfig = row["white_bishops_config"].as<int>();
        int blackPawnsConfig = row["black_pawns_config"].as<int>();
        int blackRooksConfig = row["black_rooks_config"].as<int>();
        int blackKnightsConfig = row["black_knights_config"].as<int>();
        int blackBishopsConfig = row["black_bishops_config"].as<int>();
        long long whitePawns = row["white_pawns"].as<long long>();
        long long whiteRooks = row["white_rooks"].as<long long>();
        long long whiteKnights = row["white_knights"].as<long long>();
        long long whiteBishops = row["white_bishops"].as<long long>();
        long long whiteQueens = row["white_queens"].as<long long>();
        long long whiteKings = row["white_kings"].as<long long>();
        long long blackPawns = row["black_pawns"].as<long long>();
        long long blackRooks = row["black_rooks"].as<long long>();
        long long blackKnights = row["black_knights"].as<long long>();
        long long blackBishops = row["black_bishops"].as<long long>();
        long long blackQueens = row["black_queens"].as<long long>();
        long long blackKings = row["black_kings"].as<long long>();
        int turn = row["turn"].as<int>();

        EssenceArgs essenceArgs;
        essenceArgs.whitePawn = (PieceEssence)whitePawnsConfig;
        essenceArgs.whiteRook = (PieceEssence)whiteRooksConfig;
        essenceArgs.whiteKnight = (PieceEssence)whiteKnightsConfig;
        essenceArgs.whiteBishop = (PieceEssence)whiteBishopsConfig;
        essenceArgs.blackPawn = (PieceEssence)blackPawnsConfig;
        essenceArgs.blackRook = (PieceEssence)blackRooksConfig;
        essenceArgs.blackKnight = (PieceEssence)blackKnightsConfig;
        essenceArgs.blackBishop = (PieceEssence)blackBishopsConfig;

        BitBoard pieces[12];
        pieces[getPieceIndex(White, Pawn)].value = whitePawns;
        pieces[getPieceIndex(White, Rook)].value = whiteRooks;
        pieces[getPieceIndex(White, Knight)].value = whiteKnights;
        pieces[getPieceIndex(White, Bishop)].value = whiteBishops;
        pieces[getPieceIndex(White, Queen)].value = whiteQueens;
        pieces[getPieceIndex(White, King)].value = whiteKings;
        pieces[getPieceIndex(Black, Pawn)].value = blackPawns;
        pieces[getPieceIndex(Black, Rook)].value = blackRooks;
        pieces[getPieceIndex(Black, Knight)].value = blackKnights;
        pieces[getPieceIndex(Black, Bishop)].value = blackBishops;
        pieces[getPieceIndex(Black, Queen)].value = blackQueens;
        pieces[getPieceIndex(Black, King)].value = blackKings;

        Board board;
        board.initBoard(essenceArgs, pieces, (PieceColor)turn);
        int score = board.evalBoard(White);

        txn.exec_params(R"(
            UPDATE public.board_results
	        SET score = $3
	        WHERE board_hash = $1 AND id_essence_config = $2;
        )", boardHash, idEssenceConfig, score);
        txn.commit();
    }
}

void DatabaseConnection::addBoardResult(Board board, int idEssenceConfig, GameResult gameResult)
{
    int score = board.evalBoard(White);

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
            (board_hash, id_essence_config, score, white_count)
            VALUES
            ($1, $2, $3, 1)
            ON CONFLICT (board_hash, id_essence_config)
            DO UPDATE SET white_count = board_results.white_count + 1
        )", (long long)board.hash.value, idEssenceConfig, score);
        break;
    case BlackWin:
        txn.exec_params(R"(
            INSERT INTO public.board_results
            (board_hash, id_essence_config, score, black_count)
            VALUES
            ($1, $2, $3, 1)
            ON CONFLICT (board_hash, id_essence_config)
            DO UPDATE SET black_count = board_results.black_count + 1;
        )", (long long)board.hash.value, idEssenceConfig, score);
        break;
    case Stalemate:
        txn.exec_params(R"(
            INSERT INTO public.board_results
            (board_hash, id_essence_config, score, stalemate_count)
            VALUES
            ($1, $2, $3, 1)
            ON CONFLICT (board_hash, id_essence_config)
            DO UPDATE SET stalemate_count = board_results.stalemate_count + 1;
        )", (long long)board.hash.value, idEssenceConfig, score);
        break;
    }
    txn.commit();
}
