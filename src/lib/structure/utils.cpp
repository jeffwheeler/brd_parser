#include "utils.h"

#include <algorithm>

auto x01_center(const T01ArcSegment<kAMax>* inst) -> std::pair<double, double> {
  return {static_cast<double>(inst->x), static_cast<double>(inst->y)};
}

auto ordered_stackup_materials(File<kAMax>& f)
    -> const std::vector<stackup_material> {
  std::vector<stackup_material> arr;
  for (auto& [k, stackup_material] : f.stackup_materials) {
    arr.push_back(stackup_material);
  }

  std::sort(begin(arr), end(arr), [](auto const& a, auto const& b) {
    return a.layer_id < b.layer_id;
  });

  return arr;
}

auto padtype(PadType padtype) -> std::string {
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
