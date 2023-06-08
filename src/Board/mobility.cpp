#include "mobility.h"

DefaultMobilityFlags::DefaultMobilityFlags() : MobilityFlags(false, false, false, false, false, false, false)
{
}

EnPassantMoveFlags::EnPassantMoveFlags() : MobilityFlags(true, true, false, false, false, true, false)
{
}

EnPassantCaptureFlags::EnPassantCaptureFlags() : MobilityFlags(false, false, false, false, false, false, true)
{
}

QueenSideCastleFlags::QueenSideCastleFlags() : MobilityFlags(true, true, true, true, false, false, false)
{
}

KingSideCastleFlags::KingSideCastleFlags() : MobilityFlags(true, true, true, false, true, false, false)
{
}

MobilityFlags::MobilityFlags(bool onlyLastIsValid, bool initiative, bool cowardly, bool callQueenRook, bool callKingRook, bool hasty, bool vigilant)
    : onlyLastIsValid(onlyLastIsValid), initiative(initiative), cowardly(cowardly), callQueenRook(callQueenRook), callKingRook(callKingRook), hasty(hasty), vigilant(vigilant)
{
}

Mobility::Mobility(MovementType type, MobilityFlags flags, int start_x, int start_y, int direction_x, int direction_y, int limit)
    : type(type), flags(flags), start_x(start_x), start_y(start_y), direction_x(direction_x), direction_y(direction_y), limit(limit)
{
}
