#ifndef UCI_H
#define UCI_H

#include "game.h"
#include "string"

class uci {
public:
	void Run();
private:
	Game* curGame;
	Essence whitePawnEssence = Classic; Essence whiteRookEssence = Classic; Essence whiteKnightEssence = Classic; Essence whiteBishopEssence = Classic;
	Essence blackPawnEssence = Classic; Essence blackRookEssence = Classic; Essence blackKnightEssence = Classic; Essence blackBishopEssence = Classic;
	void SendMobilities(Piece* curPiece);
	bool ParseCommand(std::string command);
	PieceType uci::StringToPieceType(std::string value);
	std::string uci::PieceTypeToString(PieceType value);
	Essence uci::StringToEssence(std::string value);
	std::string uci::EssenceToString(Essence value);
	std::string LegalMoveToString(LegalMove* value);
};

#endif
