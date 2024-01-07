#include "board.h"
#include <iostream>

void Board::initBoard(EssenceArgs essenceArgs)
{
	setEssenceConfig(essenceArgs);

	clearBoard();

	for (int i = 0; i < 8; i++) {
		addPiece(White, Pawn, i, 1);
		addPiece(Black, Pawn, i, 6);
	}

	addPiece(White, Rook, 0, 0);
	addPiece(White, Rook, 7, 0);
	addPiece(Black, Rook, 0, 7);
	addPiece(Black, Rook, 7, 7);

	addPiece(White, Knight, 1, 0);
	addPiece(White, Knight, 6, 0);
	addPiece(Black, Knight, 1, 7);
	addPiece(Black, Knight, 6, 7);

	addPiece(White, Bishop, 2, 0);
	addPiece(White, Bishop, 5, 0);
	addPiece(Black, Bishop, 2, 7);
	addPiece(Black, Bishop, 5, 7);

	addPiece(White, Queen, 3, 0);
	addPiece(Black, Queen, 3, 7);

	addPiece(White, King, 4, 0);
	addPiece(Black, King, 4, 7);

	refreshAggregations();
	notMoved.value = allPieces.value;

	curTurn = White;
	MoveGenerator::generateMoves(this);
}

void Board::setEssenceConfig(EssenceArgs essenceArgs)
{
	essenceConfig[White][Pawn] = essenceArgs.whitePawn;
	essenceConfig[White][Rook] = essenceArgs.whiteRook;
	essenceConfig[White][Knight] = essenceArgs.whiteKnight;
	essenceConfig[White][Bishop] = essenceArgs.whiteBishop;
	essenceConfig[White][Queen] = Classic;
	essenceConfig[White][King] = Classic;

	essenceConfig[Black][Pawn] = essenceArgs.blackPawn;
	essenceConfig[Black][Rook] = essenceArgs.blackRook;
	essenceConfig[Black][Knight] = essenceArgs.blackKnight;
	essenceConfig[Black][Bishop] = essenceArgs.blackBishop;
	essenceConfig[Black][Queen] = Classic;
	essenceConfig[Black][King] = Classic;
}

void Board::clearBoard()
{
	for (int color = 0; color < 2; color++) {
		for (int type = 0; type < 6; type++) {
			auto& bitboard = pieces[color][type];
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
		std::cout << move.toString() << " ";
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
		mgMobScore += Evaluation::mg_pieceMobWeights[type] * (eval.mobCount[White][type] - pieceCounts[White][type] * Evaluation::pieceMobPenalties[type]);
		mgMobScore -= Evaluation::mg_pieceMobWeights[type] * (eval.mobCount[Black][type] - pieceCounts[Black][type] * Evaluation::pieceMobPenalties[type]);

		egMobScore += Evaluation::eg_pieceMobWeights[type] * (eval.mobCount[White][type] - pieceCounts[White][type] * Evaluation::pieceMobPenalties[type]);
		egMobScore -= Evaluation::eg_pieceMobWeights[type] * (eval.mobCount[Black][type] - pieceCounts[Black][type] * Evaluation::pieceMobPenalties[type]);
	}

	int mgTropismScore = 0;
	int egTropismScore = 0;

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			auto result = getPiece(y, x);
			if (result.first) {
				Piece piece = result.second;

				if (piece.color == White) {
					mgTropismScore -= pieces[opponent[piece.color]][King].getTropism(y, x) * Evaluation::mg_pieceTropismWeights[piece.type];
					egTropismScore -= pieces[opponent[piece.color]][King].getTropism(y, x) * Evaluation::eg_pieceTropismWeights[piece.type];
				} else {
					mgTropismScore += pieces[opponent[piece.color]][King].getTropism(y, x) * Evaluation::mg_pieceTropismWeights[piece.type];
					egTropismScore += pieces[opponent[piece.color]][King].getTropism(y, x) * Evaluation::eg_pieceTropismWeights[piece.type];
				}
			}
		}
	}

	int mgScore = mgPcsqScore + mgMobScore + mgTropismScore;
	int egScore = egPcsqScore + egMobScore + egTropismScore;

	score += (curPhase * mgScore + (Evaluation::startPhase - curPhase) * egScore) / Evaluation::startPhase;

	// revert score if we are calculating for back

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
			auto& bitboard = pieces[c][t];

			if (bitboard.getBit(x, y)) {
				PieceColor color = (PieceColor)c;
				PieceType type = (PieceType)t;
				PieceEssence essence = essenceConfig[c][t];

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
			auto& bitboard = pieces[c][t];

			if (bitboard.getBit(x, y)) {
				bitboard.clearBit(x, y);
				
				PieceColor color = (PieceColor)c;
				PieceType type = (PieceType)t;
				PieceEssence essence = essenceConfig[c][t];

				notMoved.clearBit(x, y);
				pieceCounts[color][type]--;
				eval.matEval[color] -= Evaluation::pieceMatValues[type];
				eval.mg_pcsqEval[color] -= Evaluation::get_mg_pcsq(color, type, x, y);
				eval.eg_pcsqEval[color] -= Evaluation::get_eg_pcsq(color, type, x, y);

				return std::make_pair(true, Piece{ color, type, essence });
			}
		}
	}

	return std::make_pair(false, Piece{});
}

void Board::addPiece(PieceColor color, PieceType type, char x, char y)
{
	pieces[color][type].setBit(x, y);
	pieceCounts[color][type]++;
	eval.matEval[color] += Evaluation::pieceMatValues[type];
	eval.mg_pcsqEval[color] += Evaluation::get_mg_pcsq(color, type, x, y);
	eval.eg_pcsqEval[color] += Evaluation::get_eg_pcsq(color, type, x, y);
}

void Board::refreshAggregations()
{
	colors[White].value = pieces[White][Pawn].value | pieces[White][Rook].value | pieces[White][Knight].value | pieces[White][Bishop].value | pieces[White][Queen].value | pieces[White][King].value;
	colors[Black].value = pieces[Black][Pawn].value | pieces[Black][Rook].value | pieces[Black][Knight].value | pieces[Black][Bishop].value | pieces[Black][Queen].value | pieces[Black][King].value;
	allPieces.value = colors[White].value | colors[Black].value;
}

bool Board::makeMove(char x1, char y1, char x2, char y2, PieceType promotion)
{
	for (LegalMove& move : getLegalMoves()) {
		if (move.x1 == x1 && move.y1 == y1 && move.x2 == x2 && move.y2 == y2 && move.promotion == promotion)
			return makeMove(move);
	}

	return false;
}

bool Board::makeMove(LegalMove move)
{
	switch (move.castling) {
	case::kingSide:
		switch (curTurn) {
		case::White:
			removePiece(0, 0);
			removePiece(4, 0);
			addPiece(White, King, 1, 0);
			addPiece(White, Rook, 2, 0);
			break;
		case::Black:
			removePiece(0, 7);
			removePiece(4, 7);
			addPiece(Black, King, 1, 7);
			addPiece(Black, Rook, 2, 7);
			break;
		}
		break;
	case::queenSide:
		switch (curTurn) {
		case::White:
			removePiece(4, 0);
			removePiece(7, 0);
			addPiece(White, Rook, 5, 0);
			addPiece(White, King, 6, 0);
			break;
		case::Black:
			removePiece(4, 7);
			removePiece(7, 7);
			addPiece(Black, Rook, 5, 7);
			addPiece(Black, King, 6, 7);
			break;
		}
		break;
	default:
		auto result = removePiece(move.x1, move.y1);
		if (!result.first)
			return false; // no piece was found on source square

		auto movedPiece = result.second;
		removePiece(move.x2, move.y2);
		if (move.promotion == Pawn)
			addPiece(movedPiece.color, movedPiece.type, move.x2, move.y2);
		else
			addPiece(movedPiece.color, move.promotion, move.x2, move.y2);

		if (move.mobility.flags.vigilant && ghost.x == move.x2 && ghost.y == move.y2) {
			removePiece(ghost.parentX, ghost.parentY);
		}

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
			
		break;
	}

	refreshAggregations();
	MoveGenerator::generateMoves(this);

	bool legal = !(pieces[curTurn][King].value & attacks[opponent[curTurn]].value);
	curTurn = opponent[curTurn];

	return legal;
}

std::vector<LegalMove> Board::getLegalMoves()
{
	std::vector<LegalMove> legalMoves;
	for (LegalMove& move : moves[curTurn]) {
		Board newBoard = *this;
		if (newBoard.makeMove(move))
			legalMoves.push_back(move);
	}

	moves[curTurn] = legalMoves;
	return legalMoves;
}

std::string LegalMove::toString()
{
	std::string value;

	switch (castling) {
	case kingSide:
		value = "o-o";
		break;
	case queenSide:
		value = "o-o-o";
		break;
	default:
		value.push_back('a' + x1);
		value.push_back('1' + y1);
		value.push_back('a' + x2);
		value.push_back('1' + y2);
		break;
	}

	return value;
}
