#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#include <boost/interprocess/mapped_region.hpp>
#include <fstream>
#include <optional>

#include "lib/structure/types.h"

template <AllegroVersion version>
int8_t read_layer(File<version>& fs, uint32_t k);

template <template <AllegroVersion> typename T, AllegroVersion version>
uint32_t default_parser(File<A_174>& fs, void*& address);

template <template <AllegroVersion> typename T, AllegroVersion version>
uint32_t new_default_parser(File<A_174>& fs, void*& address);

template <AllegroVersion version>
uint32_t parse_x03(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x12(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x1C(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x1D(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x1E(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x1F(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x21(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x27(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x2A(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x31(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x35(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x36(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x38(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x3A(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x3B(File<A_174>& fs, void*& address);
template <AllegroVersion version>
uint32_t parse_x3C(File<A_174>& fs, void*& address);

template <AllegroVersion version>
struct parser_t {
    uint32_t (*parse)(File<A_174>&, void*& cur_addr);
};

template <AllegroVersion version>
const parser_t<version> PARSER_TABLE[] = {
    // 0x00
    {},
    // 0x01
    {&new_default_parser<x01, version>},
    // 0x02
    {},
    // 0x03
    {&parse_x03<version>},
    // 0x04
    {&new_default_parser<x04, version>},
    // 0x05
    {&new_default_parser<x05, version>},
    // 0x06
    {&new_default_parser<x06, version>},
    // 0x07
    {&new_default_parser<x07, version>},
    // 0x08
    {&new_default_parser<x08, version>},
    // 0x09
    {&new_default_parser<x09, version>},
    // 0x0A
    {&new_default_parser<x0A, version>},
    // 0x0B
    {},
    // 0x0C
    {&new_default_parser<x0C, version>},
    // 0x0D
    {&new_default_parser<x0D, version>},
    // 0x0E
    {&new_default_parser<x0E, version>},
    // 0x0F
    {&default_parser<x0F, version>},
    // 0x10
    {&new_default_parser<x10, version>},
    // 0x11
    {&new_default_parser<x11, version>},
    // 0x12
    {&new_default_parser<x12, version>},
    // 0x13
    {},
    // 0x14
    {&new_default_parser<x14, version>},
    // 0x15
    {&new_default_parser<x15, version>},
    // 0x16
    {&new_default_parser<x16, version>},
    // 0x17
    {&new_default_parser<x17, version>},
    // 0x18
    {},
    // 0x19
    {},
    // 0x1A
    {},
    // 0x1B
    {&new_default_parser<x1B, version>},
    // 0x1C
    {&parse_x1C<version>},
    // 0x1D
    {&parse_x1D<version>},
    // 0x1E
    {&parse_x1E<version>},
    // 0x1F
    {&parse_x1F<version>},
    // 0x20
    {&new_default_parser<x20, version>},
    // 0x21
    {&parse_x21<version>},
    // 0x22
    {&new_default_parser<x22, version>},
    // 0x23
    {&new_default_parser<x23, version>},
    // 0x24
    {&new_default_parser<x24, version>},
    // 0x25
    {},
    // 0x26
    {&new_default_parser<x26, version>},
    // 0x27
    {&parse_x27<version>},
    // 0x28
    {&new_default_parser<x28, version>},
    // 0x29
    {},
    // 0x2A
    {&parse_x2A<version>},
    // 0x2B
    {&new_default_parser<x2B, version>},
    // 0x2C
    {&new_default_parser<x2C, version>},
    // 0x2D
    {&new_default_parser<x2D, version>},
    // 0x2E
    {&new_default_parser<x2E, version>},
    // 0x2F
    {&new_default_parser<x2F, version>},
    // 0x30
    {&new_default_parser<x30, version>},
    // 0x31
    {&parse_x31<version>},
    // 0x32
    {&new_default_parser<x32, version>},
    // 0x33
    {&new_default_parser<x33, version>},
    // 0x34
    {&new_default_parser<x34, version>},
    // 0x35
    {&parse_x35<version>},
    // 0x36
    {&parse_x36<version>},
    // 0x37
    {&new_default_parser<x37, version>},
    // 0x38
    {&new_default_parser<x38, version>},
    // 0x39
    {&default_parser<x39, version>},
    // 0x3A
    {&default_parser<x3A, version>},
    // 0x3B
    {&parse_x3B<version>},
    // 0x3C
    {&parse_x3C<version>},
    // 0x3D
    {},
};

/*
template const parser_t<A_160>[62] PARSER_TABLE<A_160>;
template const parser_t<A_162>[62] PARSER_TABLE<A_162>;
template const parser_t<A_164>[62] PARSER_TABLE<A_164>;
template const parser_t<A_165>[62] PARSER_TABLE<A_165>;
template const parser_t<A_166>[62] PARSER_TABLE<A_166>;
template const parser_t<A_172>[62] PARSER_TABLE<A_172>;
template const parser_t<A_174>[62] PARSER_TABLE<A_174>;
*/

void skip(std::ifstream* f, std::ifstream::pos_type n);
void skip_and_pad(std::ifstream* f, std::ifstream::pos_type n);

template <AllegroVersion version>
File<A_174> parse_file_raw(boost::interprocess::mapped_region region);

// Non-template version automatically upgrades
std::optional<File<A_174>> parse_file(const std::string& filepath);

#endif
