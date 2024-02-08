#pragma once
#include <ostream>

using std::ostream;

namespace ade {

using S8 = char;
using S16 = short;
using S32 = int;
using S64 = long long;

using U8 = unsigned char;
using U16 = unsigned short;
using U32 = unsigned int;
using U64 = unsigned long long;


template<class Under, class Type>
class opaque_type {
public:
  explicit opaque_type(Under val)
    : value_(val) {}

  explicit operator Under() const noexcept {
    return value_;
  }

private:
  Under value_;
};

#define OPAQUE_TYPE(N, U, T)          \
struct N : public opaque_type<U, T> { \
    using base = opaque_type<U, T>;   \
    using base::base;                 \
};

#ifdef __GNUC__ // Packed structs without paddings
#define PACKED(...) __VA_ARGS__ __attribute__((__packed))
#elif _MSC_VER
#define PACKED(...) __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop));
#endif

extern bool starts_with(char* s, char* pre);

struct Name
{
  char ch[8];

  Name() {}
  Name(const char* s);
  operator char*();
  bool is_map_lump() const;
  friend ostream& operator << (ostream& os, const Name& name);
  friend bool operator == (const Name& a, const Name& b);
};

}
