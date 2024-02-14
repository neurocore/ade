#pragma once
#include "../../types.h"
#include <vector>

using std::vector;

namespace ade {

PACKED(struct PatchHead
{
  U16 w = 0, h = 0;
  U16 x = 0, y = 0;
});

struct Span // inside specific column
{
  U8 y_offset = 0;
  U8 count = 0;
  U8 _unused;
  vector<U8> pixels;
};

struct Patch // doom image format
{
  PatchHead head;
  vector<Span> spans;
};

}
