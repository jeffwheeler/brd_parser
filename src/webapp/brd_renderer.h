#pragma once

#include <imgui_impl_sdl2.h>

#include <memory>
#include <set>
#include <unordered_set>

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

  void UpdateFile();
  void Draw(SkSurface* surface);

  void HandleMouseWheel(const SDL_Event& event);
  void HandleMouseDown(const SDL_Event& event);
  void HandleMouseUp(const SDL_Event& event);
  void HandleMouseMove(const SDL_Event& event);
  void MarkDirty();

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
  static auto LayerToShort(uint8_t subtype, uint8_t layer) -> uint16_t;
  static auto IsPointNearPath(const SkPath& path, const SkPoint& point,
                              float width) -> bool;

  std::shared_ptr<File<kAMax>> fs_;
  std::unordered_set<uint32_t> already_drawn_;

  sk_sp<SkPicture> picture_;
  std::set<uint16_t> visible_layers_cache;

  static constexpr size_t common_width_count_ = 8;
  struct LayerPaths {
    std::array<SkPath, common_width_count_>
        common_width_paths;
    std::vector<std::pair<float, SkPath>>
        other_width_paths;

    SkPath filled_path;

    SkColor4f color;
    sk_sp<SkShader> shader;
  };
  std::array<LayerPaths, 9> shader_layers_;

  std::array<float, common_width_count_>
      common_widths_{};  // Store the actual common width values

  // Shader-related members
  sk_sp<SkRuntimeEffect> runtime_effect_;

  // Add new structure to store segment info for hover detection
  struct LineSegmentInfo {
    SkPath path;
    float width;
    uint8_t layer_id;
    uint16_t layer_short;
  };

  std::vector<LineSegmentInfo> segment_paths_;
  int hover_segment_index_ = -1;
  SkPoint current_mouse_pos_{};

  bool dirty_ = true;
  float factor_ = 1000.;
  int cached_height_ = 0.;
  float zoom_ = 1.0;
  SkPoint pan_{0, 0};
  bool is_panning_ = false;
  SkPoint last_mouse_pos_{0, 0};
};
