#include "app_state.h"

auto AppState::CurrentFile() -> std::shared_ptr<File<kAMax>>& {
  return State().fs_;
}

auto AppState::State() -> AppState& {
  static AppState state;
  return state;
}
