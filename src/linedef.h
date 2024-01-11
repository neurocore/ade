#pragma once
#include "map_format.h"

struct Linedef
{
    S16 start_vertex;
    S16 end_vertex;
    LineFlags flags;
    S16 action, tag;
    S16 front_sidedef;
    S16 back_sidedef;
};


