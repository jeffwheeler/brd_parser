#include "utils.h"

std::pair<double, double> x01_center(const x01<A_174> *inst) {
    return std::pair(cfp_to_double(inst->x), cfp_to_double(inst->y));
}
