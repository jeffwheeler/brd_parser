#include "cadence_fp.h"

#include <cmath>
#include <cstdint>
#include <cstring>

auto cfp_to_double(cadence_fp r) -> double {
  cadence_fp swapped;
  swapped.x = r.y;
  swapped.y = r.x;
  double g;
  std::memcpy(&g, &swapped, 8);
  return g;
}
