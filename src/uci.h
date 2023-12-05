#ifndef UCI_H
#define UCI_H

#include "ai.h"
#include "game.h"
#include "string"

class uci {
public:
	void run();
	bool parseCommand(std::string command);
private:
	AI* ai = new AI();
	Game* curGame;
	Essence whitePawnEssence = Classic; Essence whiteRookEssence = Classic; Essence whiteKnightEssence = Classic; Essence whiteBishopEssence = Classic;
	Essence blackPawnEssence = Classic; Essence blackRookEssence = Classic; Essence blackKnightEssence = Classic; Essence blackBishopEssence = Classic;
	void sendMobilities(Piece* curPiece);
	PieceType uci::stringToPieceType(std::string value);
	std::string uci::pieceTypeToString(PieceType value);
	Essence uci::stringToEssence(std::string value);
	std::string uci::essenceToString(Essence value);
	std::string legalMoveToString(legalMove value);
};

#endif
