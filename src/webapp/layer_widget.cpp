#include "layer_widget.h"

#include "emscripten.h"
#include "imgui.h"
#include "webapp/app_state.h"

void LayerWidget::Draw() {
  ImGui::SetNextWindowSize({350., 300.});
  ImGui::Begin("Layers");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0F / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  if (AppState::CurrentFile()) {
    ImGuiMultiSelectFlags flags =
        ImGuiMultiSelectFlags_NoAutoSelect | ImGuiMultiSelectFlags_NoAutoClear;
    ImGuiMultiSelectIO* ms_io =
        ImGui::BeginMultiSelect(flags, -1, (selected_layers_.size()));
    ImGuiSelectionExternalStorage storage_wrapper;
    storage_wrapper.UserData = (void*)&selected_layers_;
    storage_wrapper.AdapterSetItemSelected =
        [](ImGuiSelectionExternalStorage* self, int n, bool selected) {
          std::array<bool, 10> array = *(std::array<bool, 10>*)self->UserData;
          array[n] = selected;
        };
    storage_wrapper.ApplyRequests(ms_io);
    for (int n = 0; n < 10; n++) {
      char label[32];
      sprintf(label, "Copper layer %d", n);
      ImGui::SetNextItemSelectionUserData(n);
      ImGui::Checkbox(label, &selected_layers_[n]);
    }
    ms_io = ImGui::EndMultiSelect();
    storage_wrapper.ApplyRequests(ms_io);
  } else {
    ImGui::Text("No file loaded");
  }

  ImGui::End();
}
