#pragma once
#include <vector>
#include <string>
#include "../wad/vertex.h"
#include "../wad/linedef.h"
#include "../wad/sector.h"
#include "../wad/segment.h"
#include "../wad/thing.h"
#include "../imgui.h"
#include "../../internal/diff.h"

using std::vector;
using std::string;

namespace ade {

// Dummies
struct Reject {};   // maybe just a string?
struct BlockMap {}; // they are useless here

class Map
{
  string name;
  vector<Vertex> vertices;
  vector<Linedef> lines;
  vector<Sector> sectors;
  vector<Segment> segments;
  vector<Thing> things;

public:
  struct Memento
  {
    Diffs<string> name;
    Diffs<vector<Vertex>> vertices;
    Diffs<vector<Linedef>> lines;
    Diffs<vector<Sector>> sectors;
    Diffs<vector<Segment>> segments;
    Diffs<vector<Thing>> things;
  };

public:
  Memento get_updates(const Map & old) const;
  void undo_updates(const Memento& memento);
  void redo_updates(const Memento& memento);
};

}
