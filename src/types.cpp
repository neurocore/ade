#include "types.h"

namespace ade {

bool starts_with(const char* s, const char* pre)
{
  return strncmp(pre, s, strlen(pre)) == 0;
}

ostream& operator<<(ostream& os, const Name& name)
{
  for (int i = 0; i < 8; i++)
  {
    char el = name.ch[i];
    os << (el ? el : ' ');
  }
  return os;
}

bool operator==(const Name& a, const Name& b)
{
  return strcmp(a.ch, b.ch);
}

Name::Name(const char* s)
{
  memcpy_s(ch, 8, s, 8);
}

Name::operator char*()
{
  return ch;
}

bool Name::is_map_lump() const
{
  const static Name map_lumps[] =
  {
    {"THING"},
    {"LINEDEFS"},
    {"SIDEDEFS"},
    {"VERTEXES"},
    {"SEGS"},
    {"SSECTORS"},
    {"SECTORS"},
    {"NODES"},
    {"REJECT"},
    {"BLOCKMAP"},
  };

  for (const auto name : map_lumps)
    if (*this == name) return true;
  
  return false;
}

}
