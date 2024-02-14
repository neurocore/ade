#include <streambuf>
#include <iostream>
#include <iomanip>
#include "wad.h"

using namespace std;

namespace ade {

bool Wad::open(string filename)
{
  if (size > 0) return false; // not again

  ifstream file(filename, ifstream::binary);
  if (!file.good()) return false;

  file.seekg(0, ios::end); // jump to end
  size = file.tellg();     // save size
  file.seekg(0, ios::beg); // return back

  buffer = new char[size];
  file.read(buffer, size);

  read_obj(head);
  lumps = read_arr<Lump>(head.lumps_n, head.dir_offset);
  for (auto lump : lumps)
  {
    cout << lump.name << " - "
         << lump.size << "\n";
  }

  file.close();
  return true;
}

void Wad::close()
{
  size = 0;
  lumps.clear();
  head.lumps_n = 0;
  head.dir_offset = 0;
  head.type[0] = 0;
  head.type[1] = 0;
  head.type[2] = 0;
  head.type[3] = 0;

  if (buffer != nullptr)
  {
    delete[] buffer;
    buffer = nullptr;
  }
}

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
        && isdigit(lump.name[3])
        && isdigit(lump.name[4]))
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

Patch Wad::get_image(Name lump_name)
{
  Patch patch;
  if (size <= 0) return patch;

  for (U32 i = 0u; i < head.lumps_n; i++)
  {
    const Lump lump = lumps[i];
    if (lump.name == lump_name)
    {
      read_obj(patch.head, lump.offset);
      for (U32 j = 0; j < patch.head.w; j++)
      {
        U32 idx = lump.offset + sizeof(PatchHead) + j * sizeof(U32);
        U32 off; // first span offset
        memcpy(&off, buffer + idx, 4);
        off += lump.offset;

        Span span;
        for (U32 k = 0; k < 32768; k++)
        {
          span.y_offset = buffer[off++];
          if (span.y_offset == 255) break;

          span.count = buffer[off++];
          off++; // skip unused #1
          cout << (int)span.y_offset << " + "
               << (int)span.count << "\n";

          span.pixels.insert(span.pixels.end(),
                             reinterpret_cast<U8*>(buffer + off),
                             reinterpret_cast<U8*>(buffer + off) + span.count);

          patch.spans.push_back(span);
          off += span.count;
          off++; // skip unused #2

          //if (span.y_offset + span.count >= 255) break;
        }

        cout << "-----\n";
      }
    }
  }
  return patch;
}

}
