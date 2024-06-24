#include "cadence_fp.h"

#include <cmath>
#include <cstdint>
#include <cstring>

CadenceDouble::operator double() const {
  CadenceDouble swapped{.x = this->y, .y = this->x};
  double g;
  std::memcpy(&g, &swapped, 8);
  return g;
}
