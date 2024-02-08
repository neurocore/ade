#include <streambuf>
#include <iostream>
#include <iomanip>
#include "wad.h"

using namespace std;

namespace ade {

Wad::LumpRange Wad::get_map_range(int map_n)
{
  LumpRange range;
  bool in_scope = false;

  U32 i;
  for (i = 0u; i < head.lumps_n; i++)
  {
    Lump lump = lumps[i];

    if (in_scope && !lump.name.is_map_lump()) break;

    if (strncmp("MAP", lump.name, 3) == 0
    &&  isdigit(lump.name[3])
    &&  isdigit(lump.name[4]))
    {
      int num = 10 * (lump.name[3] - '0') + (lump.name[4] - '0');
      if (num == map_n)
      {
        in_scope = true;
        range.start = i + 1;
      }
    }
  }
  range.end = i;
  return range;
}

bool Wad::read()
{
  if (init) return false;

  ifstream file(filename, ifstream::binary);
  if (!file.good()) return false;

  file.seekg(0, ios::end);  // find size,
  auto size = file.tellg(); // reserve and
  file.seekg(0, ios::beg);  // return back

  buffer = new char[size];
  file.read(buffer, size);
  read_obj(buffer, head);

  lumps = read_arr<Lump>(buffer, head.lumps_n, head.dir_offset);
  for (auto lump : lumps)
  {
    cout << lump.name << " - "
         << lump.size << "\n";
  }

  file.close();
  init = true;
  return true;
}

}
