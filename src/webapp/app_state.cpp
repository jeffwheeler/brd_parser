#include "app_state.h"

auto AppState::CurrentFile() -> std::shared_ptr<File<kAMax>>& {
  return State().fs_;
}

auto AppState::VisibleLayers() -> std::set<uint16_t>& {
  return State().visible_layers_;
}

auto AppState::State() -> AppState& {
  static AppState state;
  return state;
}
