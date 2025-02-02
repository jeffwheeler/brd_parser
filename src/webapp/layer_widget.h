#pragma once

#include <array>
#include <vector>

class LayerWidget {
 public:
  LayerWidget() = default;

  void UpdateFile();
  void Draw();

 private:
  struct FilmWithSelection {
    std::string name;
    uint32_t k;
    bool selected;
  };

  std::vector<FilmWithSelection> film_list_;
};
