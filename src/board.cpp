#include <board.h>
#include <iostream>


void Board::initBoard(EssenceArgs essenceArgs)
{
	setEssenceConfig(essenceArgs);

	clearBoard();

	for (int i = 0; i < 8; i++) {
		addPiece(White, Pawn, i, 1, true);
		addPiece(Black, Pawn, i, 6, true);
	}

	addPiece(White, Rook, 0, 0, true);
	addPiece(White, Rook, 7, 0, true);
	addPiece(Black, Rook, 0, 7, true);
	addPiece(Black, Rook, 7, 7, true);

	addPiece(White, Knight, 1, 0, true);
	addPiece(White, Knight, 6, 0, true);
	addPiece(Black, Knight, 1, 7, true);
	addPiece(Black, Knight, 6, 7, true);

	addPiece(White, Bishop, 2, 0, true);
	addPiece(White, Bishop, 5, 0, true);
	addPiece(Black, Bishop, 2, 7, true);
	addPiece(Black, Bishop, 5, 7, true);

	addPiece(White, Queen, 3, 0, true);
	addPiece(Black, Queen, 3, 7, true);

	addPiece(White, King, 4, 0, true);
	addPiece(Black, King, 4, 7, true);

	refreshAggregations();

	halfmoveClock = 0;
	fullmoveClock = 1;

	curTurn = White;
	MoveGenerator::generateMoves(this);
}

void Board::setEssenceConfig(EssenceArgs essenceArgs)
{
	essenceConfig[getPieceIndex(White, Pawn)] = essenceArgs.whitePawn;
	essenceConfig[getPieceIndex(White, Rook)] = essenceArgs.whiteRook;
	essenceConfig[getPieceIndex(White, Knight)] = essenceArgs.whiteKnight;
	essenceConfig[getPieceIndex(White, Bishop)] = essenceArgs.whiteBishop;
	essenceConfig[getPieceIndex(White, Queen)] = Classic;
	essenceConfig[getPieceIndex(White, King)] = Classic;

	essenceConfig[getPieceIndex(Black, Pawn)] = essenceArgs.blackPawn;
	essenceConfig[getPieceIndex(Black, Rook)] = essenceArgs.blackRook;
	essenceConfig[getPieceIndex(Black, Knight)] = essenceArgs.blackKnight;
	essenceConfig[getPieceIndex(Black, Bishop)] = essenceArgs.blackBishop;
	essenceConfig[getPieceIndex(Black, Queen)] = Classic;
	essenceConfig[getPieceIndex(Black, King)] = Classic;
}

void Board::clearBoard()
{
	for (int color = 0; color < 2; color++) {
		for (int type = 0; type < 6; type++) {
			auto& bitboard = pieces[getPieceIndex((PieceColor)color, (PieceType)type)];
			bitboard.value = 0;
		}
	}

	eval = EvalArgs{};
}

void Board::printBoard()
{
	for (int y = 7; y >= 0; y--) {
		if (y != 7)
			std::cout << "-------------------------------" << std::endl;

		for (int x = 0; x < 8; x++) {
			char color = 'X';
			char essence = 'X';
			char type = 'X';

			auto result = getPiece(x, y);
			if (result.first) {
				auto piece = result.second;

				color = colorChars[piece.color];
				essence = essenceChars[piece.essence];
				type = typeChars[piece.type];
			}

			if (x != 0)
				std::cout << "|";
			std::cout << color << essence << type;
		}

		std::cout << std::endl;
	}
}

void Board::printMoves()
{
	for (LegalMove& move : getLegalMoves()) {
		std::cout << move.toStringWithFlags(curTurn) << " ";
	}

	std::cout << std::endl;
}

int Board::evalBoard(PieceColor color)
{
	int score = 0;

	// evaluation independent from game phase

	int matScore = eval.matEval[White] - eval.matEval[Black];

	if (eval.attCount[White] < 2)
		eval.attWeight[White] = 0;
	if (eval.attCount[Black] < 2)
		eval.attWeight[Black] = 0;

	int safetyScore = Evaluation::safetyTable[eval.attWeight[White]] - Evaluation::safetyTable[eval.attWeight[Black]];

	score += matScore + safetyScore;

	// evaluation dependent on game phase

	int curPhase = eval.curPhase;
	if (curPhase > Evaluation::startPhase)
		curPhase = Evaluation::startPhase;

	int mgPcsqScore = eval.mg_pcsqEval[White] - eval.mg_pcsqEval[Black];
	int egPcsqScore = eval.eg_pcsqEval[White] - eval.eg_pcsqEval[Black];

	int mgMobScore = 0;
	int egMobScore = 0;

	for (int type = 0; type < 6; type++) {
		char whitePieceIndex = getPieceIndex(White, type);
		char blackPieceIndex = getPieceIndex(Black, type);

		mgMobScore += Evaluation::mg_pieceMobWeights[type] * (eval.mobCount[whitePieceIndex] - pieceCounts[whitePieceIndex] * Evaluation::pieceMobPenalties[type]);
		mgMobScore -= Evaluation::mg_pieceMobWeights[type] * (eval.mobCount[blackPieceIndex] - pieceCounts[blackPieceIndex] * Evaluation::pieceMobPenalties[type]);

		egMobScore += Evaluation::eg_pieceMobWeights[type] * (eval.mobCount[whitePieceIndex] - pieceCounts[whitePieceIndex] * Evaluation::pieceMobPenalties[type]);
		egMobScore -= Evaluation::eg_pieceMobWeights[type] * (eval.mobCount[blackPieceIndex] - pieceCounts[blackPieceIndex] * Evaluation::pieceMobPenalties[type]);
	}

	int mgTropismScore = 0;
	int egTropismScore = 0;

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			auto result = getPiece(y, x);
			if (result.first) {
				Piece piece = result.second;

				char kingIndex = getPieceIndex(opponent[piece.color], King);

				if (piece.color == White) {
					mgTropismScore -= pieces[kingIndex].getTropism(y, x) * Evaluation::mg_pieceTropismWeights[piece.type];
					egTropismScore -= pieces[kingIndex].getTropism(y, x) * Evaluation::eg_pieceTropismWeights[piece.type];
				} else {
					mgTropismScore += pieces[kingIndex].getTropism(y, x) * Evaluation::mg_pieceTropismWeights[piece.type];
					egTropismScore += pieces[kingIndex].getTropism(y, x) * Evaluation::eg_pieceTropismWeights[piece.type];
				}
			}
		}
	}

	int mgScore = mgPcsqScore + mgMobScore + mgTropismScore;
	int egScore = egPcsqScore + egMobScore + egTropismScore;

	score += (curPhase * mgScore + (Evaluation::startPhase - curPhase) * egScore) / Evaluation::startPhase;
	if (color == Black)
		score *= -1;

	return score;
}

bool Board::isQuiet()
{
	return !(colors[White].value & attacks[Black].value || colors[Black].value & attacks[White].value);
}

std::pair<bool, Piece> Board::getPiece(char x, char y)
{
	for (int c = 0; c < 2; c++) {
		for (int t = 0; t < 6; t++) {
			char pieceIndex = getPieceIndex(c, t);
			auto& bitboard = pieces[pieceIndex];

			if (bitboard.getBit(x, y)) {
				PieceColor color = (PieceColor)c;
				PieceType type = (PieceType)t;
				PieceEssence essence = essenceConfig[pieceIndex];

				return std::make_pair(true, Piece{ color, type, essence });
			}
		}
	}

	return std::make_pair(false, Piece{});
}

std::pair<bool, Piece> Board::removePiece(char x, char y)
{
	for (int c = 0; c < 2; c++) {
		for (int t = 0; t < 6; t++) {
			char pieceIndex = getPieceIndex(c, t);
			auto& bitboard = pieces[pieceIndex];

			if (bitboard.getBit(x, y)) {
				bitboard.clearBit(x, y);
				
				PieceColor color = (PieceColor)c;
				PieceType type = (PieceType)t;
				PieceEssence essence = essenceConfig[pieceIndex];

				notMoved.clearBit(x, y);
				pieceCounts[pieceIndex]--;

				eval.matEval[color] -= Evaluation::pieceMatValues[type];
				eval.mg_pcsqEval[color] -= Evaluation::get_mg_pcsq(color, type, x, y);
				eval.eg_pcsqEval[color] -= Evaluation::get_eg_pcsq(color, type, x, y);

				hash.switchSquare({ color, type }, x, y);
				if (notMoved.getBit(x, y))
					hash.switchNotMoved(x, y);

				return std::make_pair(true, Piece{ color, type, essence });
			}
		}
	}

	return std::make_pair(false, Piece{});
}

void Board::addPiece(PieceColor color, PieceType type, char x, char y, bool isNew)
{
	char pieceIndex = getPieceIndex(color, type);
	pieces[pieceIndex].setBit(x, y);
	pieceCounts[pieceIndex]++;

	eval.matEval[color] += Evaluation::pieceMatValues[type];
	eval.mg_pcsqEval[color] += Evaluation::get_mg_pcsq(color, type, x, y);
	eval.eg_pcsqEval[color] += Evaluation::get_eg_pcsq(color, type, x, y);

	hash.switchSquare({ color, type }, x, y);
	if (isNew) {
		notMoved.setBit(x, y);
		hash.switchNotMoved(x, y);
	}
}

void Board::refreshAggregations()
{
	colors[White].value = pieces[getPieceIndex(White, Pawn)].value;
	colors[White].value |= pieces[getPieceIndex(White, Rook)].value;
	colors[White].value |= pieces[getPieceIndex(White, Knight)].value;
	colors[White].value |= pieces[getPieceIndex(White, Bishop)].value;
	colors[White].value |= pieces[getPieceIndex(White, Queen)].value;
	colors[White].value |= pieces[getPieceIndex(White, King)].value;

	colors[Black].value = pieces[getPieceIndex(Black, Pawn)].value;
	colors[Black].value |= pieces[getPieceIndex(Black, Rook)].value;
	colors[Black].value |= pieces[getPieceIndex(Black, Knight)].value;
	colors[Black].value |= pieces[getPieceIndex(Black, Bishop)].value;
	colors[Black].value |= pieces[getPieceIndex(Black, Queen)].value;
	colors[Black].value |= pieces[getPieceIndex(Black, King)].value;

	allPieces.value = colors[White].value | colors[Black].value;
}

bool Board::makeMove(char x1, char y1, char x2, char y2)
{
	return makeMove(x1, y1, x2, y2, Pawn);
}

bool Board::makeMove(char x1, char y1, char x2, char y2, PieceType promotion)
{
	for (LegalMove& move : moves[curTurn]) {
		if (move.x1 == x1 && move.y1 == y1 && move.x2 == x2 && move.y2 == y2 && move.promotion == promotion)
			return makeMove(move);
	}

	return false;
}

bool Board::makeMove(LegalMove move)
{
	bool halfmoveClockReset = false;

	switch (move.castling) {
	case::kingSide:
		switch (curTurn) {
		case::White:
			removePiece(0, 0);
			removePiece(4, 0);
			addPiece(White, King, 1, 0, false);
			addPiece(White, Rook, 2, 0, false);
			break;
		case::Black:
			removePiece(0, 7);
			removePiece(4, 7);
			addPiece(Black, King, 1, 7, false);
			addPiece(Black, Rook, 2, 7, false);
			break;
		}
		break;
	case::queenSide:
		switch (curTurn) {
		case::White:
			removePiece(4, 0);
			removePiece(7, 0);
			addPiece(White, Rook, 5, 0, false);
			addPiece(White, King, 6, 0, false);
			break;
		case::Black:
			removePiece(4, 7);
			removePiece(7, 7);
			addPiece(Black, Rook, 5, 7, false);
			addPiece(Black, King, 6, 7, false);
			break;
		}
		break;
	default:
		auto result = removePiece(move.x1, move.y1);
		if (!result.first)
			return false; // no piece was found on source square

		auto movedPiece = result.second;
		if (movedPiece.type == Pawn)
			halfmoveClockReset = true;

		if (removePiece(move.x2, move.y2).first)
			halfmoveClockReset = true;

		if (move.promotion == Pawn)
			addPiece(movedPiece.color, movedPiece.type, move.x2, move.y2, false);
		else
			addPiece(movedPiece.color, move.promotion, move.x2, move.y2, false);

		if (move.mobility.flags.vigilant && ghost.x == move.x2 && ghost.y == move.y2) {
			if (removePiece(ghost.parentX, ghost.parentY).first)
				halfmoveClockReset = true;
		}
			
		break;
	}

	hash.switchGhost(ghost);

	if (move.mobility.flags.hasty) {
		int parentX = move.x2;
		int parentY = move.y2;
		int ghostX = parentX;
		int ghostY = parentY;

		switch (curTurn) {
		case White:
			parentX -= move.mobility.direction_x;
			parentY -= move.mobility.direction_y;
			break;
		case Black:
			parentX += move.mobility.direction_x;
			parentY += move.mobility.direction_y;
			break;
		}

		ghost = { ghostX, ghostY, parentX, parentY };
	} else {
		ghost = {};
	}

	hash.switchGhost(ghost);

	refreshAggregations();
	MoveGenerator::generateMoves(this);

	bool legal = !(pieces[getPieceIndex(curTurn, King)].value & attacks[opponent[curTurn]].value);

	if (halfmoveClockReset)
		halfmoveClock = 0;
	else
		halfmoveClock++;

	if (curTurn == Black)
		fullmoveClock++;

	curTurn = opponent[curTurn];
	hash.switchTurn();

	return legal;
}

std::vector<LegalMove> Board::getLegalMoves()
{
	std::vector<LegalMove> legalMoves;

	if (movesValidated) {
		legalMoves = moves[curTurn];
	} else {
		for (LegalMove& move : moves[curTurn]) {
			Board newBoard = *this;
			if (newBoard.makeMove(move))
				legalMoves.push_back(move);
		}

		moves[curTurn] = legalMoves;
	}

	return legalMoves;
}

std::string Board::toString()
{
	std::string fen = "";
	
	// piece placement
	int emptyCounter = 0;
	for (int y = 7; y >= 0; y--) {
		if (y < 7)
			fen += "/";

		for (int x = 0; x < 8; x++) {
			auto result = getPiece(x, y);

			if (result.first) {
				if (emptyCounter > 0) {
					fen += std::to_string(emptyCounter);
					emptyCounter = 0;
				}

				fen += result.second.toChar();
			} else {
				emptyCounter++;
			}
		}

		if (emptyCounter > 0) {
			fen += std::to_string(emptyCounter);
			emptyCounter = 0;
		}
	}

	fen += " ";

	// active color
	switch (curTurn) {
	case White:
		fen += "w";
		break;
	case Black:
		fen += "b";
		break;
	}

	fen += " ";

	// castling rights
	char whiteKingIndex = getPieceIndex(White, King);
	char whiteRookIndex = getPieceIndex(White, Rook);
	char blackKingIndex = getPieceIndex(Black, King);
	char blackRookIndex = getPieceIndex(Black, Rook);

	// white king-side castle
	if (pieces[whiteKingIndex].getBit(4, 0) && notMoved.getBit(4, 0)
		&& pieces[whiteRookIndex].getBit(7, 0) && notMoved.getBit(7, 0)) {
		fen += "K";
	}

	// white queen-side castle
	if (pieces[whiteKingIndex].getBit(4, 0) && notMoved.getBit(4, 0)
		&& pieces[whiteRookIndex].getBit(0, 0) && notMoved.getBit(0, 0)) {
		fen += "Q";
	}

	// black king-side castle
	if (pieces[blackKingIndex].getBit(4, 7) && notMoved.getBit(4, 7)
		&& pieces[blackRookIndex].getBit(7, 7) && notMoved.getBit(7, 7)) {
		fen += "k";
	}

	// black queen-side castle
	if (pieces[blackKingIndex].getBit(4, 7) && notMoved.getBit(4, 7)
		&& pieces[blackRookIndex].getBit(0, 7) && notMoved.getBit(0, 7)) {
		fen += "q";
	}

	fen += " ";

	// En Passant targets
	if (ghost.x < 0 || ghost.y < 0) {
		fen += "-";
	} else {
		char column = 'a' + ghost.x;
		char row = '0' + ghost.y;

		fen += column;
		fen += row;
	}

	fen += " ";

	// halfmove clock
	fen += std::to_string(halfmoveClock);

	fen += " ";

	// fullmove clock
	fen += std::to_string(fullmoveClock);

	return fen;
}

GameResult Board::getResult()
{
	if (getLegalMoves().empty()) {
		if (curTurn == White && (pieces[getPieceIndex(White, King)].value & attacks[Black].value))
			return BlackWin;
		else if (curTurn == Black && (pieces[getPieceIndex(Black, King)].value & attacks[White].value))
			return WhiteWin;
		else
			return Stalemate;
	} else if (halfmoveClock == 100) {
		return Stalemate;
	} else {
		return Unresolved;
	}
}
