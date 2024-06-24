#ifndef CADENCE_FP_H
#define CADENCE_FP_H

#include <cmath>
#include <cstdint>

/* Cadence seems to use a double for storing some types, but the first and
 * second 32 bit words seem to be flipped. This type provides a function to
 * swap them.
 */
typedef struct {
  uint32_t x;
  uint32_t y;
} cadence_fp;

auto cfp_to_double(cadence_fp r) -> double;

#endif
