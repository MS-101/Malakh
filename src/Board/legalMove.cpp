#include "legalMove.h"

LegalMove::LegalMove(int x1, int y1, int x2, int y2, PieceType promotionType)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->promotionType = promotionType;
}
