#include "app_state.h"

auto AppState::CurrentFile() -> std::unique_ptr<File<kAMax>>& {
  return State().fs_;
}

auto AppState::VisibleLayers() -> std::unordered_set<LayerInfo>& {
  return State().visible_layers_;
}

auto AppState::State() -> AppState& {
  static AppState state;
  return state;
}

void AppState::RecordPicture() { State().needs_recording_ = true; }

void AppState::PictureRecordingDone() { State().needs_recording_ = false; }

auto AppState::PictureNeedsRecording() -> bool {
  return State().needs_recording_;
}
