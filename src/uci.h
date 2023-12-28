#ifndef UCI_H
#define UCI_H

#include "ai.h"
#include "string"

class uci {
public:
	void run();
	bool parseCommand(std::string command);
	std::string legalMoveToString(LegalMove value, PieceColor color);
private:
	AI* ai = new AI();
	Board* board = nullptr;
	EssenceConfig essenceConfig{};
	void sendMobilities(Piece* curPiece);
	PieceType uci::stringToPieceType(std::string value);
	std::string uci::pieceTypeToString(PieceType value);
	Essence uci::stringToEssence(std::string value);
	std::string uci::essenceToString(Essence value);
	
};

#endif
