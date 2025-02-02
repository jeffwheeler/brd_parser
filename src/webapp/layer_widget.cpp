#include "layer_widget.h"

#include <cstring>

#include "emscripten.h"
#include "imgui.h"
#include "lib/structure/utils.h"
#include "webapp/app_state.h"

void LayerWidget::UpdateFile() {
  // Reload film list cache
  film_list_.clear();

  auto file = AppState::CurrentFile();
  if (file) {
    for (const auto& [name, k] : film_list(*file)) {
      film_list_.emplace_back(
          FilmWithSelection{.name = name, .k = k, .selected = false});
    }
  }
}

void LayerWidget::Draw() {
  ImGui::SetNextWindowSize({350., 300.}, ImGuiCond_FirstUseEver);
  ImGui::Begin("Layers");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0F / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  if (AppState::CurrentFile()) {
    ImGuiMultiSelectFlags flags =
        ImGuiMultiSelectFlags_NoAutoSelect | ImGuiMultiSelectFlags_NoAutoClear;
    ImGuiMultiSelectIO* ms_io =
        ImGui::BeginMultiSelect(flags, -1, (film_list_.size()));
    ImGuiSelectionExternalStorage storage_wrapper;
    storage_wrapper.UserData = static_cast<void*>(&film_list_);
    storage_wrapper.AdapterSetItemSelected =
        [](ImGuiSelectionExternalStorage* self, int n, bool selected) {
          auto array = *(std::vector<FilmWithSelection>*)self->UserData;
          array[n].selected = selected;

          // Reset the visible layers cache
          auto file = AppState::CurrentFile();
          auto& visible_layers = AppState::VisibleLayers();
          visible_layers.clear();

          for (auto const& film : array) {
            if (film.selected) {
              const T38Film<kAMax>& film_inst = file->get_t38_film(film.k);
              const T39FilmLayerList<kAMax>& layer_list =
                  file->get_x39(film_inst.layer_list);
              for (const auto& [x, y] : x39_layers(layer_list, *file)) {
                visible_layers.insert(LayerInfo({.family = x, .id = y}));
              }
            }
          }
        };
    storage_wrapper.ApplyRequests(ms_io);
    uint8_t i = 0;
    for (auto& film : film_list_) {
      char label[64];
      strncpy(label, film.name.c_str(), sizeof(label));
      ImGui::SetNextItemSelectionUserData(i++);
      ImGui::Checkbox(label, &film.selected);
    }
    ms_io = ImGui::EndMultiSelect();
    storage_wrapper.ApplyRequests(ms_io);
  } else {
    ImGui::Text("No file loaded");
  }

  ImGui::End();
}
