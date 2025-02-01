#pragma once

#include <imgui_impl_sdl2.h>

#include <memory>
#include <unordered_set>

#include "include/core/SkBBHFactory.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkPath.h"
#include "include/core/SkPicture.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkSurface.h"
#include "include/effects/SkRuntimeEffect.h"
#include "lib/structure/types.h"

class BrdWidget {
 public:
  BrdWidget();

  void UpdateFile(std::shared_ptr<File<kAMax>> fs);
  void Draw(SkSurface* surface, double device_pixel_ratio_,
            std::array<bool, 10>& selected_layers);

  void HandleMouseWheel(const SDL_Event& event);
  void HandleMouseDown(const SDL_Event& event);
  void HandleMouseUp(const SDL_Event& event);
  void HandleMouseMove(const SDL_Event& event);

 private:
  void IterateFile();
  void ComposeLayersToDrawable();
  void InitializeShader();
  void UpdateLayerShaders();
  void UpdateLayerAlpha(uint8_t layer, float new_alpha);

  void DrawShape(uint32_t ptr);
  void DrawX05(const T05Line<kAMax>* inst);
  void DrawX28(const T28Shape<kAMax>* inst);

  auto StartingPoint(uint32_t k) -> std::optional<SkPoint>;
  auto IsLineSegment(uint32_t k) -> bool;
  auto GetWidthIndex(float width) -> size_t;

  auto ScreenToWorld(const SkPoint& screen_pos) -> SkPoint;
  static auto LayerToId(uint8_t subtype, uint8_t layer) -> uint8_t;
  static auto IsPointNearPath(const SkPath& path, const SkPoint& point,
                              float width) -> bool;

  std::shared_ptr<File<kAMax>> fs_;
  std::unordered_set<uint32_t> already_drawn_;

  sk_sp<SkPicture> picture_;
  std::array<bool, 10> selected_layers_cache_{};

  static constexpr size_t common_width_count_ = 8;
  struct LayerPaths {
    std::array<SkPath, common_width_count_>
        common_width_paths;  // Paths for most common widths
    std::vector<std::pair<float, SkPath>>
        other_width_paths;  // Less common widths

    // For filled paths (shapes) - no width needed
    SkPath filled_path;
  };
  std::array<LayerPaths, 9> layer_paths_;
  std::array<float, common_width_count_>
      common_widths_{};  // Store the actual common width values

  // Shader-related members
  sk_sp<SkRuntimeEffect> runtime_effect_;
  std::array<SkColor4f, 9> layer_colors_{};
  std::array<sk_sp<SkShader>, 9> layer_shaders_;

  // Add new structure to store segment info for hover detection
  struct LineSegmentInfo {
    SkPath path;
    float width;
    uint8_t layer_id;
  };

  std::vector<LineSegmentInfo> segment_paths_;
  int hover_segment_index_ = -1;
  SkPoint current_mouse_pos_{};

  float factor_ = 1000.;
  int cached_height_ = 0.;
  float zoom_ = 1.0;
  SkPoint pan_{0, 0};
  bool is_panning_ = false;
  SkPoint last_mouse_pos_{0, 0};
};
