#pragma once
#include <vector>
#include "../wad/vertex.h"
#include "../wad/linedef.h"
#include "../wad/sector.h"
#include "../wad/thing.h"

using std::vector;

namespace ade {

// Dummies
struct Sidedef {};
struct SubSector {};
struct Node {};
struct Reject {};
struct BlockMap {};

struct Map
{
  Name name;
  vector<Vertex> vertices;
  vector<Linedef> lines;
  vector<Sector> sectors;
  vector<Thing> things;

};

class Ade // project
{

};

}
