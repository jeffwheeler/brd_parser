#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Half.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Platform/EmscriptenApplication.h>
#include <Magnum/Trade/MeshData.h>
#include <imgui_impl_sdl2.h>

#include <unordered_set>
#include <vector>

#include "lib/structure/types.h"
#include "webapp/line_shader.h"

namespace Mn = Magnum;

using namespace Mn::Math::Literals;

class BrdWidget {
 public:
  BrdWidget();

  void UpdateFile();
  void Draw();

  void UpdateScreenRatio();
  void HandleMouseWheel(
      Mn::Platform::EmscriptenApplication::ScrollEvent& event);
  void HandleMouseDown(
      Mn::Platform::EmscriptenApplication::PointerEvent& event);
  void HandleMouseUp(Mn::Platform::EmscriptenApplication::PointerEvent& event);
  void HandleMouseMove(
      Mn::Platform::EmscriptenApplication::PointerMoveEvent& event);
  void MarkDirty();

 private:
  void IterateFile();
  void UpdateLayerAlpha(uint8_t layer, float new_alpha);

  void DrawShape(uint32_t ptr);
  void DrawX05(const T05Line<kAMax>* inst);
  void DrawX28(const T28Shape<kAMax>* inst);

  // Used for T15LineSegment, T16LineSegment, or T17LineSegment
  template <class T>
  void DrawSegment(const T* inst, Mn::Half width, uint8_t layer_id);

  void AddSegment(Mn::Vector2 start, Mn::Vector2 end, Mn::Half width,
                  uint8_t layer);
  void AddArc(const T01ArcSegment<kAMax>& segment_inst, Mn::Half width,
              uint8_t layer_id);
  void AddLineCap(Mn::Vector2 start, Mn::Vector2 end, Mn::Half width,
                  uint8_t layer);
  auto StartingPoint(uint32_t k) -> std::optional<Mn::Vector2>;
  auto IsLineSegment(uint32_t k) -> bool;

  auto ScreenToWorld(const Mn::Vector2& screen_pos, bool center = false)
      -> Mn::Vector2;
  static auto LayerToShader(LayerInfo layer) -> uint8_t;
  // static auto IsPointNearPath(const SkPath& path, const SkPoint& point,
  //                             float width) -> bool;

  std::shared_ptr<File<kAMax>> fs_;
  std::unordered_set<uint32_t> already_drawn_;

  struct VertexData {
    Mn::Vector2 position;
    Mn::Vector2 next;
    Mn::Half width;
    Mn::Byte step;
    Mn::Byte layer_id;
  };

  Mn::GL::Buffer buffer;
  Mn::GL::Mesh mesh_;
  LineShader _lineShader;

  std::vector<VertexData> lines_cache_;

  std::array<Mn::Color4, 32> layer_colors_{};
  std::array<float, 32> layer_opacities_{};
  // sk_sp<SkPicture> picture_;
  std::unordered_set<LayerInfo> visible_layers_cache_;

  // This cannot be `constexpr` because the `Half` literal is not `constexpr`.
  Mn::Half border_width_ = 0.003_h;

  constexpr static float kNormalOpacity = 0.9F;
  constexpr static float kShadowOpacity = 0.15F;

  // FIXME: Could this be max radians instead?
  constexpr static uint8_t kArcSegmentDivision = 8;

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
  float factor_ = 500000.;
  // int cached_height_ = 0.;
  // float zoom_ = 1.0;
  // Mn::Vector2 pan_{0, 0};
  bool is_panning_ = false;
  Mn::Vector2 last_mouse_pos_{0, 0};

  Mn::Matrix3 transformation_matrix_, projection_matrix_, aspect_ratio_matrix_;
};
