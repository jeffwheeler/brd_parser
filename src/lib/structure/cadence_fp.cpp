#include "cadence_fp.h"

#include <cmath>
#include <cstdint>
#include <cstring>

auto cfp_to_double(CadenceDouble r) -> double {
  CadenceDouble swapped;
  swapped.x = r.y;
  swapped.y = r.x;
  double g;
  std::memcpy(&g, &swapped, 8);
  return g;
}
