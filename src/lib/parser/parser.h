#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#include <fstream>
#include <optional>

#include "lib/structure/types.h"

template <AllegroVersion version>
int8_t read_layer(File<version>& fs, uint32_t k);

template <template <AllegroVersion> typename T, AllegroVersion version>
uint32_t default_parser(File<version>& fs, std::ifstream& f);

template <AllegroVersion version>
uint32_t parse_x03(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x12(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x1C(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x1D(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x1E(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x1F(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x21(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x27(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x2A(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x31(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x35(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x36(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x38(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x3A(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x3B(File<version>& fs, std::ifstream& f);
template <AllegroVersion version>
uint32_t parse_x3C(File<version>& fs, std::ifstream& f);

template <AllegroVersion version>
struct parser_t {
    uint32_t (*parse)(File<version>&, std::ifstream&);
};

template <AllegroVersion version>
const parser_t<version> PARSER_TABLE[] = {
    // 0x00
    {},
    // 0x01
    {.parse = &default_parser<x01>},
    // 0x02
    {},
    // 0x03
    {.parse = &parse_x03},
    // 0x04
    {.parse = &default_parser<x04>},
    // 0x05
    {.parse = &default_parser<x05>},
    // 0x06
    {.parse = &default_parser<x06>},
    // 0x07
    {.parse = &default_parser<x07>},
    // 0x08
    {.parse = &default_parser<x08>},
    // 0x09
    {.parse = &default_parser<x09>},
    // 0x0A
    {.parse = &default_parser<x0A>},
    // 0x0B
    {},
    // 0x0C
    {.parse = &default_parser<x0C>},
    // 0x0D
    {.parse = &default_parser<x0D>},
    // 0x0E
    {.parse = &default_parser<x0E>},
    // 0x0F
    {.parse = &default_parser<x0F>},
    // 0x10
    {.parse = &default_parser<x10>},
    // 0x11
    {.parse = &default_parser<x11>},
    // 0x12
    {.parse = &parse_x12},
    // 0x13
    {},
    // 0x14
    {.parse = &default_parser<x14>},
    // 0x15
    {.parse = &default_parser<x15>},
    // 0x16
    {.parse = &default_parser<x16>},
    // 0x17
    {.parse = &default_parser<x17>},
    // 0x18
    {},
    // 0x19
    {},
    // 0x1A
    {},
    // 0x1B
    {.parse = &default_parser<x1B>},
    // 0x1C
    {.parse = &parse_x1C},
    // 0x1D
    {.parse = &parse_x1D},
    // 0x1E
    {.parse = &parse_x1E},
    // 0x1F
    {.parse = &parse_x1F},
    // 0x20
    {.parse = default_parser<x20>},
    // 0x21
    {.parse = &parse_x21},
    // 0x22
    {.parse = &default_parser<x22>},
    // 0x23
    {.parse = &default_parser<x23>},
    // 0x24
    {.parse = &default_parser<x24>},
    // 0x25
    {},
    // 0x26
    {.parse = &default_parser<x26>},
    // 0x27
    {.parse = &parse_x27},
    // 0x28
    {.parse = &default_parser<x28>},
    // 0x29
    {},
    // 0x2A
    {.parse = &parse_x2A},
    // 0x2B
    {.parse = &default_parser<x2B>},
    // 0x2C
    {.parse = &default_parser<x2C>},
    // 0x2D
    {.parse = &default_parser<x2D>},
    // 0x2E
    {.parse = &default_parser<x2E>},
    // 0x2F
    {},
    // 0x30
    {.parse = &default_parser<x30>},
    // 0x31
    {.parse = &parse_x31},
    // 0x32
    {.parse = &default_parser<x32>},
    // 0x33
    {.parse = &default_parser<x33>},
    // 0x34
    {.parse = &default_parser<x34>},
    // 0x35
    {.parse = &parse_x35},
    // 0x36
    {.parse = &parse_x36},
    // 0x37
    {.parse = &default_parser<x37>},
    // 0x38
    {.parse = &default_parser<x38>},
    // 0x39
    {.parse = &default_parser<x39>},
    // 0x3A
    {.parse = &default_parser<x3A>},
    // 0x3B
    {.parse = &parse_x3B},
    // 0x3C
    {.parse = &parse_x3C},
    // 0x3D
    {},
};

void skip(std::ifstream* f, std::ifstream::pos_type n);
void skip_and_pad(std::ifstream* f, std::ifstream::pos_type n);

template <AllegroVersion version>
File<version> parse_file(const std::string& filepath);

// Non-template version automatically upgrades
std::optional<File<A_174>> parse_file(const std::string& filepath);

#endif
