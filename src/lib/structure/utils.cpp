#include "utils.h"

#include <algorithm>

std::pair<double, double> x01_center(const x01<A_174>* inst) {
    return std::pair(cfp_to_double(inst->x), cfp_to_double(inst->y));
}

const std::vector<stackup_material> ordered_stackup_materials(File<A_174>& f) {
    std::vector<stackup_material> arr;
    for (auto& [k, stackup_material] : f.stackup_materials) {
        arr.push_back(stackup_material);
    }

    std::sort(begin(arr), end(arr), [](auto const& a, auto const& b) {
        return a.layer_id < b.layer_id;
    });

    return arr;
}

std::string padtype(PadType padtype) {
    switch (padtype) {
        case PadType::ThroughVia:
            return "ThroughVia";
        case PadType::Via:
            return "Via";
        case PadType::SmtPin:
            return "SmtPin";
        case PadType::Slot:
            return "Slot";
        case PadType::HoleA:
            return "HoleA";
        case PadType::HoleB:
            return "HoleB";
        default:
            return "PadTypeUnrecognized";
    }
}
