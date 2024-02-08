#pragma once
#include "../../types.h"
#include "../../domain/complevel.h"

namespace ade {

PACKED(struct Linedef
{
  U16 start_vertex;
  U16 end_vertex;
  LineFlags flags;
  U16 action, tag;
  U16 front_sidedef;
  U16 back_sidedef;
});

}
