#include "app_state.h"

auto AppState::CurrentFile() -> std::shared_ptr<File<kAMax>>& {
  return State().fs_;
}

auto AppState::VisibleLayers() -> std::unordered_set<LayerInfo>& {
  return State().visible_layers_;
}

auto AppState::State() -> AppState& {
  static AppState state;
  return state;
}
