#ifndef UCI_H
#define UCI_H

#include "game.h"


class uci
{
public:
	void Run();
private:
	Game* curGame;
	Essence whitePawnEssence = Classic; Essence whiteRookEssence = Classic; Essence whiteKnightEssence = Classic; Essence whiteBishopEssence = Classic;
	Essence blackPawnEssence = Classic; Essence blackRookEssence = Classic; Essence blackKnightEssence = Classic; Essence blackBishopEssence = Classic;
	void SendMobilities(Piece* curPiece);
	bool ParseCommand(std::string command);
	std::string GetAvailableMoves(Piece* curPiece);
	Essence uci::StringToEssence(std::string value);
	std::string uci::EssenceToString(Essence value);
};

#endif
