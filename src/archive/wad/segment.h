#pragma once
#include "../../types.h"

namespace ade {

PACKED(struct Segment
{
  U16 vertex_start_id;
  U16 vertex_end_id;
  U16 angle;
  U16 linedef_id;
  U16 same_dir;
  U16 offset;

  friend auto operator<=>(const Segment&, const Segment&) = default;
});

struct Node {};

}
