#pragma once
#include "../../types.h"
#include "../../domain/complevel.h"

namespace ade {

PACKED(struct Sector
{
  U16 floor_height;
  U16 ceiling_height;
  Name floor_tex;
  Name ceiling_tex;
  U16 brightness;
  Specials special;
  U16 tag;

  friend auto operator<=>(const Sector&, const Sector&) = default;
});

struct SubSector {};

}
