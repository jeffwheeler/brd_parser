#pragma once

#include <memory>

#include "lib/structure/types.h"

class LayerWidget {
 public:
  LayerWidget() : fs_(nullptr) {};

  void UpdateFile(std::shared_ptr<File<kAMax>> fs);
  void Draw();

  // Show a few layers by default
  std::array<bool, 10> selected_layers_ = {true,  false, true,  false, false,
                                           false, false, false, false, false};

 private:
  std::shared_ptr<File<kAMax>> fs_;
};
