#ifndef PRINTERS_H
#define PRINTERS_H

#if _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <cstdint>

#include "lib/structure/types.h"

#define PRINT_ALL_ITEMS 0
#define MAX_DEPTH 5

#define PRINT(MAP, KEY, DEPTH) \
  (print_struct((const void *)&fs->MAP.at(KEY), fs, DEPTH))

template <AllegroVersion version>
void default_printer(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x01(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x03(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x04(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x05(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x06(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x07(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x08(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x09(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x0A(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x0C(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x0D(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x0F(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x10(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x11(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x12(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x14(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x15(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x16(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x17(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x1B(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_t13(const t13<version> &t13_inst, File<version> *fs, int d, int i);

template <AllegroVersion version>
void print_x1C(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x1E(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x22(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x23(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x24(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x26(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x28(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x2A(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x2B(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x2C(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x2D(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x30(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x31(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x32(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x33(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x34(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x36(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x37(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x38(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x39(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x3A(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
void print_x3C(const void *untyped_inst, File<version> *fs, int d);

template <AllegroVersion version>
struct sfs {
  void (*print)(const void *, File<version> *, const int);
};

template <AllegroVersion version>
void print_struct(const void *untyped_inst, File<version> *fs, int depth);

template <AllegroVersion version>
void print_struct(uint32_t k, File<version> &fs, int d);

#endif
