#include "mobility.h"

Mobility::Mobility(MovementType type, MobilityFlags flags, int start_x, int start_y, int direction_x, int direction_y, int limit)
{
    this->type = type;
    this->flags = flags;
    this->start_x = start_x;
    this->start_y = start_y;
    this->direction_x = direction_x;
    this->direction_y = direction_y;
    this->limit = limit;
}
