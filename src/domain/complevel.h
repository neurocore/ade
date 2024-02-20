#pragma once
#include <string>
#include <bitset>
#include "../types.h"

namespace ade {

using LineFlags = U16; // for linedefs
using Specials = U16; // for sectors

class CompLevel {
public:
  virtual bool is_match(LineFlags flags) = 0;
  virtual void readMap(const std::string& filename) = 0;
  virtual void processMap() = 0;
};


class CL2 : public CompLevel {
public:
  static constexpr LineFlags BlockAll{ 0x0001 };
  static constexpr LineFlags BlockMonsters{ 0x0002 };
  static constexpr LineFlags TwoSided{ 0x0004 };
  static constexpr LineFlags UpperUnpegged{ 0x0008 };
  static constexpr LineFlags LowerUnpegged{ 0x0010 };
  static constexpr LineFlags ShowOneSided{ 0x0020 };
  static constexpr LineFlags BlockSound{ 0x0040 };
  static constexpr LineFlags NeverShow{ 0x0080 };
  static constexpr LineFlags AlwaysShow{ 0x0100 };

  static constexpr Specials Normal{ 0 };
  static constexpr Specials BlinkRandom{ 1 };
  static constexpr Specials BlinkHalf{ 2 };
  static constexpr Specials BlinkSecond{ 3 };
  static constexpr Specials Damage20Blink{ 4 };
  static constexpr Specials Damage10{ 5 };
  static constexpr Specials _Reserved1{ 6 };
  static constexpr Specials Damage5{ 7 };
  static constexpr Specials LightGlow{ 8 };
  static constexpr Specials Secret{ 9 };
  static constexpr Specials CloseDoor30Sec{ 10 };
  static constexpr Specials Damage20Exit{ 11 };
  static constexpr Specials BlinkSyncSec{ 12 };
  static constexpr Specials BlinkSyncHalf{ 13 };
  static constexpr Specials CloseDoor300Sec{ 14 };
  static constexpr Specials _Reserved2{ 15 };
  static constexpr Specials Damage20{ 16 };
  static constexpr Specials LightFlickers{ 17 };

  bool is_match(LineFlags flags) override { return true; }
  void readMap(const std::string& filename) override {}
  void processMap() override {}
};


class CL9 : public CL2 {
public:
  static constexpr LineFlags PassThru{ 0x0200 };

  static constexpr Specials BoomDamage5{ 32 };
  static constexpr Specials BoomDamage10{ 64 };
  static constexpr Specials BoomDamage20{ 96 }; // 5 & 6 bits enabled!
  static constexpr Specials BoomSecret{ 128 };
  static constexpr Specials BoomFriction{ 256 };
  static constexpr Specials BoomPushPull{ 512 };

  bool is_match(LineFlags flags) override { return true; }
  void readMap(const std::string& filename) override {}
  void processMap() override {}
};


class CL11 : public CL9 {
public:
  bool is_match(LineFlags flags) override { return true; }
  void readMap(const std::string& filename) override {}
  void processMap() override {}
};


class CL21 : public CL11 {
public:
  static constexpr LineFlags BlockLandMobs{ 0x1000 };
  static constexpr LineFlags BlockPlayers{ 0x2000 };

  static constexpr Specials MBF21AlternateDamage{ 4096 };
  static constexpr Specials MBF21KillGroundedMobs{ 8192 };

  bool is_match(LineFlags flags) override { return true; }
  void readMap(const std::string& filename) override {}
  void processMap() override {}
};

}
