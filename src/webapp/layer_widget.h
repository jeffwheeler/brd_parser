#pragma once

#include <array>

class LayerWidget {
 public:
  LayerWidget() = default;

  void Draw();

  // Show a few layers by default
  std::array<bool, 10> selected_layers_ = {true,  false, true,  false, false,
                                           false, false, false, false, false};
};
