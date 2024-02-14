#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../../types.h"
#include "patch.h"

using std::ifstream;
using std::string;
using std::vector;

namespace ade {

class Wad
{
  enum Scope
  {
    Global  = 0,
    Map     = 1 << 0,
    Walls   = 1 << 1,
    Flats   = 1 << 2,
    Sprites = 1 << 3
  };

  PACKED(struct Head
  {
    char type[4];
    U32 lumps_n = 0;
    U32 dir_offset = 0;
  });

  PACKED(struct Lump
  {
    U32 offset = 0;
    U32 size = 0;
    Name name;
  });

  struct LumpRange
  {
    U32 start = 0;
    U32 end = 0;
  };

public:
  ~Wad() { close(); }
  bool is_good() const { return size > 0; }
  operator bool() const { return is_good(); }
  bool open(string filename);
  void close();

  U32 get_lumps_n() { return is_good() ? head.lumps_n : 0; }
  LumpRange get_map_range(int map_n);

  template<class T>
  vector<T> get_map_table(LumpRange range, Name lump_name);

  template<class T>
  bool read_obj(T& obj, U32 offset = 0u, U32 index = 0u);

  template<class T> // be sure that T is packed
  vector<T> read_arr(U32 count, U32 offset = 0u);

  /*template<class T>
  vector<T> get_entities(Scope scope);*/

  Patch get_image(Name lump_name);

private:
  U64 size = 0;
  char * buffer;
  Head head;
  vector<Lump> lumps;
};

template<class T> // be sure that T is packed
bool Wad::read_obj(T& obj, U32 offset, U32 index)
{
  if (offset + index * sizeof(T) < 0) return false;
  if (offset + (index + 1) * sizeof(T) > size) return false;

  char* result = reinterpret_cast<char*>(&obj);
  for (int i = 0; i < sizeof(T); i++)
  {
    result[i] = buffer[i + offset + index * sizeof(T)];
  }
  return true;
}

template<class T> // be sure that T is packed
vector<T> Wad::read_arr(U32 count, U32 offset)
{
  vector<T> objs;
  if (!size) return objs;

  objs.insert(objs.end(),
              reinterpret_cast<T*>(buffer + offset),
              reinterpret_cast<T*>(buffer + offset) + count);
  return objs;
}

template<class T>
inline vector<T> Wad::get_map_table(LumpRange range, Name lump_name)
{
  if (size > 0)
  for (U32 i = range.start; i < range.end; i++)
  {
    if (lumps[i].name == lump_name)
    {
      return read_arr<T>(lumps[i].size / sizeof(T),
                         lumps[i].offset);
    }
  }
  return vector<T>();
}

}
