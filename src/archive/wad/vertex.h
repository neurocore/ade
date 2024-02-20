#pragma once
#include "../../types.h"

namespace ade {

PACKED(struct Vertex
{
  S16 x, y;

  friend auto operator<=>(const Thing&, const Thing&) = default;
});

}
