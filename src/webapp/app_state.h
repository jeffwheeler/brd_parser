#pragma once

#include <set>

#include "lib/structure/types.h"

class AppState {
 public:
  static auto CurrentFile() -> std::shared_ptr<File<kAMax>>&;
  static auto VisibleLayers() -> std::set<uint16_t>&;

 private:
  static auto State() -> AppState&;

  std::shared_ptr<File<kAMax>> fs_;
  std::set<uint16_t> visible_layers_;
};
