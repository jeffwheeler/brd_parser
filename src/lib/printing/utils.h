#ifndef PRINTING_UTILS_H
#define PRINTING_UTILS_H

#include <stdint.h>

#include <fstream>
#include <iostream>

void log(const void* base_addr, const void* address, const char* fmt...);
void log_n_words(const void* address, uint8_t n);

void print_hex(uint32_t n);
void print_type_hint(const int d, const char* type);

void printf_d(const int d, const char* fmt...);

/************************
 * Template definitions *
 ************************/

// These must be in the header file (or `_impl.h`, which has poor clangd
// support) so they can be instantiated implicitly.

#endif
