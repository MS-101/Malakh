#include "mobility.h"

DefaultMobilityFlags::DefaultMobilityFlags() : MobilityFlags(false, false, false, false, false, false, 0, 0)
{
}

EnPassantMoveFlags::EnPassantMoveFlags() : MobilityFlags(true, true, false, false, true, false, 0, 0)
{
}

EnPassantCaptureFlags::EnPassantCaptureFlags() : MobilityFlags(false, false, false, false, false, true, 0, 0)
{
}

QueenSideCastleFlags::QueenSideCastleFlags() : MobilityFlags(true, true, true, true, false, false, -4, 0)
{
}

KingSideCastleFlags::KingSideCastleFlags() : MobilityFlags(true, true, true, true, false, false, 3, 0)
{
}

MobilityFlags::MobilityFlags(bool uninterruptible, bool initiative, bool cowardly, bool inspiring, bool hasty, bool vigilant, int affected_x, int affected_y)
    : uninterruptible(uninterruptible), initiative(initiative), cowardly(cowardly), inspiring(inspiring), hasty(hasty), vigilant(vigilant), affected_x(affected_x), affected_y(affected_y)
{
}

Mobility::Mobility(MovementType type, MobilityFlags flags, int start_x, int start_y, int direction_x, int direction_y, int limit)
    : type(type), flags(flags), start_x(start_x), start_y(start_y), direction_x(direction_x), direction_y(direction_y), limit(limit)
{
}
