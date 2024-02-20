#pragma once
#include <algorithm>
#include "../types.h"
#include "diff/dmp_diff.hpp"

namespace ade {

enum class DiffType { Del, Add, Off };

template<class T>
struct Diff
{
  using Size = typename T::size_type;

  DiffType type;
  Size size;
  T slice;
};

template<class T>
using Diffs = vector<Diff<T>>;

template<class T>
Diffs<T> get_diffs(const T& prev, const T& curr)
{
  Diffs<T> diffs;
  auto mds = MyersDiff<T>(prev, curr).diffs();
  // TODO: trivial case of whole common -> empty?

  for (auto md : mds)
  {
    Diff<T> diff;
    diff.type = static_cast<DiffType>(md.operation);
    diff.size = md.text.till - md.text.from;

    if (diff.type == DiffType::Add)
    {
      std::copy(md.text.from, md.text.till,
                std::back_inserter(diff.slice));
    }
    diffs.push_back(diff);
  }
  return diffs;
}

// [redo] abc -> ad | off_1 del_bc add_d

//  prev | curr | diffs
// ------+------+--------------------
//  .abc |  _   | off_1 del_bc add_d
//  a.bc |  a   | del_bc add_d
//  abc. |  a   | add_d
//  abc. |  ad  |

template<class T>
T redo(const T& prev, const Diffs<T>& diffs)
{
  using Iter = typename T::const_iterator;
  Iter it = prev.begin();
  T curr;

  for (auto diff : diffs)
  {
    switch (diff.type)
    {
      case DiffType::Off:
        curr.insert(curr.end(), it, it + diff.size);
        std::advance(it, diff.size);
        break;

      case DiffType::Del:
        std::advance(it, diff.size);
        break;

      case DiffType::Add:
        curr.insert(curr.end(), diff.slice.begin(), diff.slice.end());
        break;
    }
  }
  return curr;
}

// [undo] abc <- ad | off_1 del_bc add_d

//  curr | prev | diffs
// ------+------+--------------------
//  .ad  | _    | off_1 del_bc add_d
//  a.d  | a    | del_bc add_d
//  a.d  | abc  | add_d
//  ad.  | abc  |

template<class T>
T undo(const T& curr, const Diffs<T>& diffs)
{
  using Iter = typename T::const_iterator;
  Iter it = curr.begin();
  T prev;

  for (auto diff : diffs)
  {
    switch (diff.type)
    {
      case DiffType::Off:
        prev.insert(prev.end(), it, it + diff.size);
        std::advance(it, diff.size);
        break;

      case DiffType::Del:
        prev.insert(prev.end(), diff.slice.begin(), diff.slice.end());
        break;

      case DiffType::Add:
        std::advance(it, diff.size);
        break;
    }
  }
  return prev;
}

}
