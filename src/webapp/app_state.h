#pragma once

#include <unordered_set>

#include "lib/structure/types.h"

class AppState {
 public:
  static auto CurrentFile() -> std::shared_ptr<File<kAMax>>&;
  static auto VisibleLayers() -> std::unordered_set<LayerInfo>&;

  static void RecordPicture();
  static void PictureRecordingDone();
  static auto PictureNeedsRecording() -> bool;

 private:
  static auto State() -> AppState&;

  std::shared_ptr<File<kAMax>> fs_;
  std::unordered_set<LayerInfo> visible_layers_;

  bool needs_recording_;
};
