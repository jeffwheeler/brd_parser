#ifndef PRINTERS_H
#define PRINTERS_H

#include <arpa/inet.h>
#include <stdint.h>

#include "lib/structure/types.h"

#define PRINT_ALL_ITEMS 0
#define MAX_DEPTH 3

#define PRINT(MAP, KEY, DEPTH) \
    (print_struct((const void *)&fs->MAP->at(KEY), fs, DEPTH))
#define PRINT2(MAP, KEY, DEPTH) \
    (print_struct((const void *)&fs->MAP.at(KEY), fs, DEPTH))

template <AllegroVersion version>
void default_printer(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x01(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x03(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x04(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x05(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x06(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x07(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x08(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x09(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x0A(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x0C(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x0D(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x0F(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x10(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x11(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x12(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x14(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x15(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x16(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x17(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x1B(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_t13(const t13<version> &t13_inst, File<version> *fs, const int d,
               const int i);

template <AllegroVersion version>
void print_x1C(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x1E(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x22(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x23(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x26(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x28(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x2A(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x2B(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x2C(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x2D(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x30(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x31(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x32(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x33(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x34(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x36(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x37(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x38(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x39(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x3A(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
void print_x3C(const void *untyped_inst, File<version> *fs, const int d);

template <AllegroVersion version>
struct sfs {
    void (*print)(const void *, File<version> *, const int);
};

template <AllegroVersion version>
sfs<version> SFS_TABLE[] = {
    // 0x00
    {},
    // 0x01
    {.print = &print_x01},
    // 0x02
    {},
    // 0x03
    {.print = &print_x03},
    // 0x04
    {.print = &print_x04},
    // 0x05
    {.print = &print_x05},
    // 0x06
    {.print = &print_x06},
    // 0x07
    {.print = &print_x07},
    // 0x08
    {.print = &print_x08},
    // 0x09
    {.print = &print_x09},
    // 0x0A
    {.print = &print_x0A},
    // 0x0B
    {},
    // 0x0C
    {.print = &print_x0C},
    // 0x0D
    {.print = &print_x0D},
    // 0x0E
    {.print = &default_printer},
    // 0x0F
    {.print = &print_x0F},
    // 0x10
    {.print = &print_x10},
    // 0x11
    {.print = &print_x11},
    // 0x12
    {.print = &print_x12},
    // 0x13
    {},
    // 0x14
    {.print = &print_x14},
    // 0x15
    {.print = &print_x15},
    // 0x16
    {.print = &print_x16},
    // 0x17
    {.print = &print_x17},
    // 0x18
    {},
    // 0x19
    {},
    // 0x1A
    {},
    // 0x1B
    {.print = &print_x1B},
    // 0x1C
    {.print = &print_x1C},
    // 0x1D
    {},
    // 0x1E
    {.print = &print_x1E},
    // 0x1F
    {},
    // 0x20
    {},
    // 0x21
    {},
    // 0x22
    {.print = &print_x22},
    // 0x23
    {.print = &print_x23},
    // 0x24
    {.print = &default_printer},
    // 0x25
    {},
    // 0x26
    {.print = &print_x26},
    // 0x27
    {},
    // 0x28
    {.print = &print_x28},
    // 0x29
    {},
    // 0x2A
    {.print = &print_x2A},
    // 0x2B
    {.print = &print_x2B},
    // 0x2C
    {.print = &print_x2C},
    // 0x2D
    {.print = &print_x2D},
    // 0x2E
    {},
    // 0x2F
    {},
    // 0x30
    {.print = &print_x30},
    // 0x31
    {.print = &print_x31},
    // 0x32
    {.print = &print_x32},
    // 0x33
    {.print = &print_x33},
    // 0x34
    {.print = &print_x34},
    // 0x35
    {.print = default_printer},
    // 0x36
    {.print = &print_x36},
    // 0x37
    {.print = print_x37},
    // 0x38
    {.print = print_x38},
    // 0x39
    {.print = print_x39},
    // 0x3A
    {.print = print_x3A},
    // 0x3B
    {},
    // 0x3C
    {.print = print_x3C},
    // 0x3D
    {},
};

// G++ requires these forward declarations, but Clang implicitly finds them.
// This syntax does not work in Clang.
#ifndef __clang__
template sfs<A_174> SFS_TABLE<A_174>;
#endif

template <AllegroVersion version>
void print_struct(const void *untyped_inst, File<version> *fs, const int depth);

template <AllegroVersion version>
void print_struct(const uint32_t k, File<version> &fs, const int d);

#endif
