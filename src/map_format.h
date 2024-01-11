#pragma once
#include <string>
#include <bitset>
#include "types.h"

using LineFlags = std::bitset<16>;

class MapFormat {
public:
    virtual bool is_match(LineFlags flags) = 0;
    virtual void readMap(const std::string & filename) = 0;
    virtual void processMap() = 0;
};


class MapFormat2 : public MapFormat {
public:
    static const LineFlags BlockAll;
    static const LineFlags BlockMonsters;
    static const LineFlags TwoSided;
    static const LineFlags UpperUnpegged;
    static const LineFlags LowerUnpegged;
    static const LineFlags ShowOneSided;
    static const LineFlags BlockSound;
    static const LineFlags NeverShow;
    static const LineFlags AlwaysShow;

    bool is_match(LineFlags flags) override { return true; }
    void readMap(const std::string& filename) override {}
    void processMap() override {}
};

const LineFlags MapFormat2::BlockAll      { 0x0001 };
const LineFlags MapFormat2::BlockMonsters { 0x0002 };
const LineFlags MapFormat2::TwoSided      { 0x0004 };
const LineFlags MapFormat2::UpperUnpegged { 0x0008 };
const LineFlags MapFormat2::LowerUnpegged { 0x0010 };
const LineFlags MapFormat2::ShowOneSided  { 0x0020 };
const LineFlags MapFormat2::BlockSound    { 0x0040 };
const LineFlags MapFormat2::NeverShow     { 0x0080 };
const LineFlags MapFormat2::AlwaysShow    { 0x0100 };


class MapFormat9 : public MapFormat2 {
public:
    static const LineFlags PassThru;

    bool is_match(LineFlags flags) override { return true; }
    void readMap(const std::string& filename) override {}
    void processMap() override {}
};

const LineFlags MapFormat9::PassThru { 0x0200 };


class MapFormat11 : public MapFormat9 {
public:
    bool is_match(LineFlags flags) override { return true; }
    void readMap(const std::string& filename) override {}
    void processMap() override {}
};


class MapFormat21 : public MapFormat11 {
public:
    static const LineFlags BlockLandMobs;
    static const LineFlags BlockPlayers;

    bool is_match(LineFlags flags) override { return true; }
    void readMap(const std::string& filename) override {}
    void processMap() override {}
};

const LineFlags MapFormat21::BlockLandMobs { 0x1000 };
const LineFlags MapFormat21::BlockPlayers  { 0x2000 };


void check_flags(const MapFormat2 & flags) {
    /*MapFormat21::BlockLandMobs | MapFormat21::BlockPlayers;*/
}

void check_flags(const MapFormat9 & flags) {
}

int main() {
    MapFormat2 vanilla_flags;
    MapFormat9 boom_flags;

    check_flags(vanilla_flags);
    check_flags(boom_flags);

    return 0;
}
