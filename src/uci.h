#ifndef UCI_H
#define UCI_H

#include "ai.h"
#include "game.h"
#include "string"

class uci {
public:
	void run();
private:
	AI* ai = new AI();
	Game* curGame;
	Essence whitePawnEssence = Classic; Essence whiteRookEssence = Classic; Essence whiteKnightEssence = Classic; Essence whiteBishopEssence = Classic;
	Essence blackPawnEssence = Classic; Essence blackRookEssence = Classic; Essence blackKnightEssence = Classic; Essence blackBishopEssence = Classic;
	void sendMobilities(Piece* curPiece);
	bool parseCommand(std::string command);
	PieceType uci::stringToPieceType(std::string value);
	std::string uci::pieceTypeToString(PieceType value);
	Essence uci::stringToEssence(std::string value);
	std::string uci::essenceToString(Essence value);
	std::string legalMoveToString(LegalMove* value);
};

#endif
