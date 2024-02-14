#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "../wad/vertex.h"
#include "../wad/linedef.h"
#include "../wad/sector.h"
#include "../wad/thing.h"

using std::vector;
using std::string;
using std::unordered_map;

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

struct Tex
{
  U16 w, h;
  U16 x, y;


};

struct Sprite : public Tex
{

};

struct Sound
{

};

class Ade // project
{
  vector<Map> maps;
  vector<Tex> walls;
  vector<Tex> flats;
  vector<Sprite> sprites;
  unordered_map<Name, string> docs;
};

}
