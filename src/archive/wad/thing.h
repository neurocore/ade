#pragma once
#include "../../types.h"

namespace ade {

enum ThingFlags : U16
{
  SkillEasy = 1u << 0,
  SkillMedium = 1u << 1,
  SkillHard = 1u << 2,
  Ambush = 1u << 3,
  NotSingle = 1u << 4,
  NotDM = 1u << 5,
  NotCoop = 1u << 6,
  Friendly = 1u << 7,
};

PACKED(struct Thing
{
  U16 x, y;
  U16 angle;
  U16 thing_type;
  ThingFlags flags;

  friend auto operator<=>(const Thing&, const Thing&) = default;
});

}
