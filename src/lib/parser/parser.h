#ifndef PARSER_H
#define PARSER_H

#include <boost/interprocess/mapped_region.hpp>
#include <cstdint>
#include <fstream>
#include <optional>

#include "lib/structure/types.h"

template <template <AllegroVersion> typename T, AllegroVersion version>
auto default_parser(File<kAMax>& fs, void*& address) -> uint32_t;

template <AllegroVersion version>
auto parse_x03(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x12(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x1C(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x1D(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x1E(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x1F(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x21(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x27(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x2A(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x31(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x35(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x36(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x3A(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x3B(File<kAMax>& fs, void*& address) -> uint32_t;
template <AllegroVersion version>
auto parse_x3C(File<kAMax>& fs, void*& address) -> uint32_t;

using ParserFunc = uint32_t (*)(File<kAMax>&, void*&);

template <AllegroVersion version>
const std::array<ParserFunc, 0x3E> PARSER_TABLE = {
    // 0x00
    nullptr,
    // 0x01
    {&default_parser<T01ArcSegment, version>},
    // 0x02
    nullptr,
    // 0x03
    {&parse_x03<version>},
    // 0x04
    {&default_parser<x04, version>},
    // 0x05
    {&default_parser<T05Line, version>},
    // 0x06
    {&default_parser<x06, version>},
    // 0x07
    {&default_parser<T07Instance, version>},
    // 0x08
    {&default_parser<x08, version>},
    // 0x09
    {&default_parser<x09, version>},
    // 0x0A
    {&default_parser<T0ADRC, version>},
    // 0x0B
    nullptr,
    // 0x0C
    {&default_parser<T0CDrillIndicator, version>},
    // 0x0D
    {&default_parser<x0D, version>},
    // 0x0E
    {&default_parser<x0E, version>},
    // 0x0F
    {&default_parser<T0FFootprint, version>},
    // 0x10
    {&default_parser<x10, version>},
    // 0x11
    {&default_parser<x11, version>},
    // 0x12
    {&default_parser<x12, version>},
    // 0x13
    nullptr,
    // 0x14
    {&default_parser<T14Path, version>},
    // 0x15
    {&default_parser<T15LineSegment, version>},
    // 0x16
    {&default_parser<T16LineSegment, version>},
    // 0x17
    {&default_parser<T17LineSegment, version>},
    // 0x18
    nullptr,
    // 0x19
    nullptr,
    // 0x1A
    nullptr,
    // 0x1B
    {&default_parser<T1BNet, version>},
    // 0x1C
    {&parse_x1C<version>},
    // 0x1D
    {&parse_x1D<version>},
    // 0x1E
    {&parse_x1E<version>},
    // 0x1F
    {&parse_x1F<version>},
    // 0x20
    {&default_parser<x20, version>},
    // 0x21
    {&parse_x21<version>},
    // 0x22
    {&default_parser<x22, version>},
    // 0x23
    {&default_parser<T23Rat, version>},
    // 0x24
    {&default_parser<T24Rectangle, version>},
    // 0x25
    nullptr,
    // 0x26
    {&default_parser<x26, version>},
    // 0x27
    {&parse_x27<version>},
    // 0x28
    {&default_parser<T28Shape, version>},
    // 0x29
    nullptr,
    // 0x2A
    {&parse_x2A<version>},
    // 0x2B
    {&default_parser<T2BSymbol, version>},
    // 0x2C
    {&default_parser<x2C, version>},
    // 0x2D
    {&default_parser<T2DSymbolInstance, version>},
    // 0x2E
    {&default_parser<x2E, version>},
    // 0x2F
    {&default_parser<x2F, version>},
    // 0x30
    {&default_parser<T30StringGraphic, version>},
    // 0x31
    {&parse_x31<version>},
    // 0x32
    {&default_parser<T32SymbolPin, version>},
    // 0x33
    {&default_parser<x33, version>},
    // 0x34
    {&default_parser<x34, version>},
    // 0x35
    {&parse_x35<version>},
    // 0x36
    {&parse_x36<version>},
    // 0x37
    {&default_parser<x37, version>},
    // 0x38
    {&default_parser<T38Film, version>},
    // 0x39
    {&default_parser<T39FilmLayerList, version>},
    // 0x3A
    {&default_parser<T3AFilmLayerListNode, version>},
    // 0x3B
    {&parse_x3B<version>},
    // 0x3C
    {&parse_x3C<version>},
    // 0x3D
    nullptr,
};

void skip(std::ifstream* f, std::ifstream::pos_type n);
void skip_and_pad(std::ifstream* f, std::ifstream::pos_type n);

template <AllegroVersion version>
auto parse_file_raw(boost::interprocess::mapped_region region) -> File<kAMax>;

// Non-template version automatically upgrades
auto parse_file(const std::string& filepath) -> std::optional<File<kAMax>>;

#endif
