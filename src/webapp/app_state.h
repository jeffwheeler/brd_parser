#pragma once

#include "lib/structure/types.h"

class AppState {
 public:
  static auto CurrentFile() -> std::shared_ptr<File<kAMax>>&;

 private:
  static auto State() -> AppState&;

  std::shared_ptr<File<kAMax>> fs_;
};
