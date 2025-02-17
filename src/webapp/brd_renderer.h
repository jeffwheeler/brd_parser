#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Platform/EmscriptenApplication.h>
#include <Magnum/Shaders/LineGL.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/Trade/MeshData.h>
#include <imgui_impl_sdl2.h>

#include <vector>
#include <unordered_set>

#include "lib/structure/types.h"

class BrdWidget {
 public:
  BrdWidget();

  void UpdateFile();
  void Draw();

  void UpdateScreenRatio();
  void HandleMouseWheel(
      Magnum::Platform::EmscriptenApplication::ScrollEvent& event);
  void HandleMouseDown(
      Magnum::Platform::EmscriptenApplication::PointerEvent& event);
  void HandleMouseUp(
      Magnum::Platform::EmscriptenApplication::PointerEvent& event);
  void HandleMouseMove(
      Magnum::Platform::EmscriptenApplication::PointerMoveEvent& event);
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

  auto StartingPoint(uint32_t k) -> std::optional<Magnum::Vector2>;
  auto IsLineSegment(uint32_t k) -> bool;
  auto GetWidthIndex(float width) -> size_t;

  auto ScreenToWorld(const Magnum::Vector2& screen_pos, bool center = false) -> Magnum::Vector2;
  static auto LayerToShader(const LayerInfo layer) -> uint8_t;
  // static auto IsPointNearPath(const SkPath& path, const SkPoint& point,
  //                             float width) -> bool;

  std::shared_ptr<File<kAMax>> fs_;
  std::unordered_set<uint32_t> already_drawn_;

  Magnum::GL::Mesh triangle_mesh_;
  Magnum::Shaders::VertexColorGL2D triangle_shader_;

  Magnum::GL::Mesh mesh_;
  Magnum::Shaders::LineGL2D shader_;
  std::vector<Magnum::Trade::MeshData> lines_cache_;

  // sk_sp<SkPicture> picture_;
  // std::unordered_set<LayerInfo> visible_layers_cache_;

  /*
  static constexpr size_t common_width_count_ = 8;
  struct LayerPaths {
    std::array<SkPath, common_width_count_> common_width_paths;
    std::vector<std::pair<float, SkPath>> other_width_paths;

    SkPath filled_path;

    SkColor4f color;
    sk_sp<SkShader> shader;
    sk_sp<SkShader> fill_shader;
  };
  std::array<LayerPaths, 9> shader_layers_;

  std::array<float, common_width_count_> common_widths_{};

  // Shader-related members
  sk_sp<SkRuntimeEffect> runtime_effect_;

  // Add new structure to store segment info for hover detection
  struct LineSegmentInfo {
    SkPath path;
    float width;
    uint8_t layer_id;
    LayerInfo file_layer;
  };

  std::vector<LineSegmentInfo> segment_paths_;
  int hover_segment_index_ = -1;
  SkPoint current_mouse_pos_{};
  */

  bool dirty_ = true;
  float factor_ = 100000.;
  // int cached_height_ = 0.;
  // float zoom_ = 1.0;
  // Magnum::Vector2 pan_{0, 0};
  bool is_panning_ = false;
  Magnum::Vector2 last_mouse_pos_{0, 0};

  Magnum::Matrix3 transformation_matrix_, projection_matrix_, aspect_ratio_matrix_;
};
