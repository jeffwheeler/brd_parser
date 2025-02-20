#include "webapp/brd_renderer.h"

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Math/Constants.h>
#include <Magnum/Math/Functions.h>
#include <Magnum/MeshTools/CompileLines.h>
#include <Magnum/MeshTools/Concatenate.h>
#include <Magnum/MeshTools/GenerateLines.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Trade/MeshData.h>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <arpa/inet.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "lib/structure/utils.h"
#include "webapp/app_state.h"

BrdWidget::BrdWidget() : layer_colors_() {
  InitializeShader();
  UpdateScreenRatio();

  mesh_.setCount(0);

  // Reserve 500 MiB to start
  lines_cache_.reserve(500 * 1024 * 1024 / sizeof(VertexData));

  emscripten_log(EM_LOG_INFO, "Lines cache: %d MiB available",
                 sizeof(VertexData) * lines_cache_.capacity() / 1024 / 1024);

  // https://uchu.style/
  layer_colors_[0] = 0x952233AA_rgbaf;
  layer_colors_[1] = 0x00FF00AA_rgbaf;
  layer_colors_[2] = 0xAC7A8FAA_rgbaf;
  layer_colors_[3] = 0x4E288BAA_rgbaf;
  layer_colors_[4] = 0x1F48A6AA_rgbaf;
  layer_colors_[5] = 0x4E9246AA_rgbaf;
  layer_colors_[6] = 0xB09A52AA_rgbaf;
  layer_colors_[7] = 0xAA6635AA_rgbaf;
  layer_colors_[8] = 0x393B3DAA_rgbaf;

  _lineShader.setLayerColors(layer_colors_);

  for (float &layer_opacity : layer_opacities_) {
    layer_opacity = kShadowOpacity;
  }
  _lineShader.setLayerOpacities(layer_opacities_);
}

void BrdWidget::UpdateFile() {
  already_drawn_.clear();
  lines_cache_.clear();

  IterateFile();
  ComposeLayersToDrawable();

  // Add a cross to the origin
  // AddSegment({-0.5, 0}, {0.5, 0}, 0.1);
  buffer.setData(
      Cr::Containers::arrayView(lines_cache_.data(), lines_cache_.size()));

  emscripten_log(EM_LOG_INFO,
                 "Line cache: %d MiB used, sizeof(VertexData) = %d",
                 buffer.size() / 1024 / 1024, sizeof(VertexData));

  // Configure mesh
  mesh_.setPrimitive(Mn::GL::MeshPrimitive::Triangles)
      .setCount(lines_cache_.size())
      .addVertexBuffer(
          buffer, 0, LineShader::Position{}, LineShader::Next{},
          LineShader::Width{LineShader::Width::Components::One,
                            LineShader::Width::DataType::Half},
          LineShader::Step{LineShader::Step::Components::One,
                           LineShader::Step::DataType::Byte},
          LineShader::LayerId{LineShader::LayerId::Components::One,
                              LineShader::LayerId::DataType::Byte});

  dirty_ = true;
}

void BrdWidget::ComposeLayersToDrawable() {
  /*
  SkPictureRecorder recorder;

  SkRect bbox = {-10000, -10000, 10000, 10000};
  recorder.beginRecording(bbox, nullptr);

  for (const auto &shader_layer : shader_layers_) {
    // Base paint settings for strokes
    SkPaint stroke_paint;
    stroke_paint.setStyle(SkPaint::kStroke_Style);
    stroke_paint.setShader(shader_layer.shader);
    stroke_paint.setAntiAlias(true);
    stroke_paint.setStrokeCap(SkPaint::kRound_Cap);
    stroke_paint.setStrokeJoin(SkPaint::kRound_Join);
    stroke_paint.setBlendMode(SkBlendMode::kLighten);

    // Draw stroked paths
    for (size_t w = 0; w < common_width_count_; w++) {
      if (!shader_layer.common_width_paths[w].isEmpty()) {
        stroke_paint.setStrokeWidth(common_widths_[w]);
        recorder.getRecordingCanvas()->drawPath(
            shader_layer.common_width_paths[w], stroke_paint);
      }
    }

    for (const auto &[width, path] : shader_layer.other_width_paths) {
      stroke_paint.setStrokeWidth(width);
      recorder.getRecordingCanvas()->drawPath(path, stroke_paint);
    }

    if (!shader_layer.filled_path.isEmpty()) {
      SkPaint fill_paint;
      fill_paint.setStyle(SkPaint::kFill_Style);
      fill_paint.setShader(shader_layer.fill_shader);
      fill_paint.setAntiAlias(true);
      fill_paint.setBlendMode(SkBlendMode::kLighten);
      recorder.getRecordingCanvas()->drawPath(shader_layer.filled_path,
                                              fill_paint);

      fill_paint.setStyle(SkPaint::kStroke_Style);
      fill_paint.setShader(shader_layer.shader);
      fill_paint.setStrokeWidth(0.05F);
      recorder.getRecordingCanvas()->drawPath(shader_layer.filled_path,
                                              fill_paint);
    }
  }

  picture_ = recorder.finishRecordingAsPicture();
  */
}

void BrdWidget::InitializeShader() {}

void BrdWidget::UpdateLayerShaders() {}

void BrdWidget::UpdateLayerAlpha(uint8_t layer, float alpha) {
  layer_opacities_[layer] = alpha;
  _lineShader.setLayerOpacities(layer_opacities_);
}

void BrdWidget::Draw() {
  const auto &visible_layers = AppState::VisibleLayers();
  bool updated = (visible_layers_cache_ != visible_layers);
  if (updated) {
    emscripten_log(EM_LOG_INFO, "updated!");
    std::array<bool, 10> selected = {false};
    for (LayerInfo const &visible_layer : visible_layers) {
      selected[LayerToShader(visible_layer)] = true;
    }
    for (uint8_t i = 0; i < 9; i++) {
      UpdateLayerAlpha(i, selected[i] ? kNormalOpacity : kShadowOpacity);
    }
    visible_layers_cache_ = visible_layers;

    // ComposeLayersToDrawable();
    dirty_ = true;
  }

  // Draw
  _lineShader
      .setTransformationProjectionMatrix(
          projection_matrix_ * aspect_ratio_matrix_ * transformation_matrix_)
      .draw(mesh_);

  // const auto &visible_layers = AppState::VisibleLayers();

  /*
  SkCanvas *canvas = surface->getCanvas();
  cached_height_ = surface->height();

  // if (!dirty_) {
  //   return;
  // }

  dirty_ = false;

  canvas->clear(SkColorSetARGB(255, 30, 30, 30));
  canvas->save();

  SkPictureRecorder recorder;
  recorder.beginRecording(surface->width(), surface->height(), nullptr);
  auto *record_canvas = recorder.getRecordingCanvas();

  record_canvas->translate(pan_.x() * zoom_, pan_.y() * zoom_);
  record_canvas->scale(zoom_, zoom_);

  // Draw base picture
  record_canvas->drawPicture(picture_.get());
  auto local_picture = recorder.finishRecordingAsPicture();

  if (AppState::PictureNeedsRecording()) {
    emscripten_log(EM_LOG_INFO, "Recording one image");
    AppState::PictureRecordingDone();

    auto data = local_picture->serialize();
    std::string_view view(static_cast<const char*>(data->data()), data->size());

    std::string filename{"recording.skp"};
    std::string mime_type{"application/text/plain"};
    emscripten_browser_file::download(filename, mime_type, view);
  }

  canvas->drawPicture(local_picture);

  // Draw hover effect if applicable
  if (hover_segment_index_ >= 0) {
    const auto &segment = segment_paths_[hover_segment_index_];
    SkPaint hover_paint;
    hover_paint.setStyle(SkPaint::kStroke_Style);
    hover_paint.setColor(SkColorSetARGB(255, 255, 255, 0));  // Yellow highlight
    hover_paint.setStrokeWidth(segment.width *
                               1.1F);  // Slightly wider than original
    hover_paint.setAntiAlias(true);
    hover_paint.setStrokeCap(SkPaint::kRound_Cap);
    hover_paint.setStrokeJoin(SkPaint::kRound_Join);
    hover_paint.setBlendMode(SkBlendMode::kPlus);

    canvas->drawPath(segment.path, hover_paint);
  }

  canvas->restore();
  */
}

void BrdWidget::MarkDirty() { dirty_ = true; }

void BrdWidget::IterateFile() {
  emscripten_log(EM_LOG_INFO, "IterateFile");

  for (const auto &i_x1B : AppState::CurrentFile()->iter_t1B_net()) {
    for (const auto &i_x04 : AppState::CurrentFile()->iter_x04(i_x1B.k)) {
      uint32_t k = i_x04.ptr2;
      while (true) {
        if (AppState::CurrentFile()->is_type(k, 0x33)) {
          const auto &i = AppState::CurrentFile()->get_x33(k);
          // printf("- - Found x33 w/ key = 0x %08X\n", ntohl(k));
          // drawShape(k, pen3);
          k = i.un1;
        } else if (AppState::CurrentFile()->is_type(k, 0x32)) {
          const auto &i = AppState::CurrentFile()->get_x32(k);
          // printf("- - Found x32 w/ key = 0x %08X\n", ntohl(k));
          k = i.un1;
        } else if (AppState::CurrentFile()->is_type(k, 0x2E)) {
          const auto &i = AppState::CurrentFile()->get_x2E(k);
          // printf("- - Found x2E w/ key = 0x %08X\n", ntohl(k));
          k = i.un[0];
        } else if (AppState::CurrentFile()->is_type(k, 0x28)) {
          const auto &x = AppState::CurrentFile()->get_x28(k);
          DrawShape(x.k);
          k = x.next;
        } else if (AppState::CurrentFile()->is_type(k, 0x1B)) {
          break;
        } else if (AppState::CurrentFile()->is_type(k, 0x0E)) {
          const auto &x = AppState::CurrentFile()->get_x0E(k);
          k = x.un[0];
        } else if (AppState::CurrentFile()->is_type(k, 0x05)) {
          const auto &x = AppState::CurrentFile()->get_x05(k);
          DrawShape(x.k);
          k = x.ptr0;
        } else if (AppState::CurrentFile()->is_type(k, 0x04)) {
          break;
        } else {
          printf("- - \x1b[31mUnexpected key\x1b[0m = 0x %08X :(\n", ntohl(k));
          break;
        }
      }
    }
  }
}

void BrdWidget::AddArc(const T01ArcSegment<kAMax> &segment_inst, Mn::Half width,
                       uint8_t layer_id) {
  auto [cx, cy] = x01_center(&segment_inst);
  float scaled_cx = cx / factor_;
  float scaled_cy = cy / factor_;
  float scaled_r = (static_cast<double>(segment_inst.r)) / factor_;

  Magnum::Vector2 start{segment_inst.coords[0] / factor_,
                        segment_inst.coords[1] / factor_};
  Magnum::Vector2 end{segment_inst.coords[2] / factor_,
                      segment_inst.coords[3] / factor_};

  Mn::Radd start_angle{atan2(start.y() - scaled_cy, start.x() - scaled_cx)};
  Mn::Radd end_angle{atan2(end.y() - scaled_cy, end.x() - scaled_cx)};

  if (segment_inst.subtype == 0) {  // Clockwise or counterclockwise
    if (end_angle < start_angle) {
      end_angle = end_angle + Mn::Radd{Mn::Math::Constants<float>::tau()};
    }
  } else {
    if (end_angle > start_angle) {
      end_angle = end_angle - Mn::Radd{Mn::Math::Constants<float>::tau()};
    }
  }

  Magnum::Radd angle_step = (end_angle - start_angle) / kArcSegmentDivision;

  Mn::Vector2 prev_point = {
      static_cast<float>(scaled_cx + (scaled_r * Mn::Math::cos(start_angle))),
      static_cast<float>(scaled_cy + (scaled_r * Mn::Math::sin(start_angle)))};
  for (uint8_t i = 1; i <= kArcSegmentDivision; i++) {
    Magnum::Radd theta = start_angle + angle_step * i;
    Mn::Vector2 next_point = {
        static_cast<float>(scaled_cx + (scaled_r * Mn::Math::cos(theta))),
        static_cast<float>(scaled_cy + (scaled_r * Mn::Math::sin(theta)))};

    AddSegment(prev_point, next_point, width, layer_id);
    prev_point = next_point;
  }
}

void BrdWidget::DrawShape(uint32_t ptr) {
  if (already_drawn_.count(ptr) > 0) {
    return;
  }

  already_drawn_.insert(ptr);

  if (AppState::CurrentFile()->is_type(ptr, 0x05)) {
    // std::printf("Trying to draw x05\n");
    const T05Line<kAMax> inst = AppState::CurrentFile()->get_x05(ptr);
    DrawX05(&inst);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x0C)) {
    // const T0CDrillIndicator<kAMax> inst =
    // AppState::CurrentFile()->get_x0C(ptr); drawX0C(&inst, pen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x10)) {
    // const x10<kAMax> inst = AppState::CurrentFile()->get_x10(ptr);
    // drawShape(inst.ptr1, darkerPen);
    // drawShape(inst.ptr2, darkerPen);
    // drawShape(inst.ptr3, darkerPen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x14)) {
    // const T14Path<kAMax> inst = AppState::CurrentFile()->get_x14(ptr);
    // drawX14(&inst, pen);
    // } else if (AppState::CurrentFile()->x15_map.count(ptr) > 0) {
    // const x15<A_MAX> *inst = (const x15<A_MAX>
    // *)&AppState::CurrentFile()->x15_map.at(ptr); drawX15(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
    // } else if (AppState::CurrentFile()->x16_map.count(ptr) > 0) {
    // const x16<A_MAX> *inst = (const x16<A_MAX>
    // *)&AppState::CurrentFile()->x16_map.at(ptr); drawX16(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
    // } else if (AppState::CurrentFile()->x17_map.count(ptr) > 0) {
    // const x17<A_MAX> *inst = (const x17<A_MAX>
    // *)&AppState::CurrentFile()->x17_map.at(ptr); drawX17(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x23)) {
    // const T23Rat<kAMax> inst = AppState::CurrentFile()->get_x23(ptr);
    // drawX23(&inst, pen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x24)) {
    // const T24Rectangle<kAMax> inst = AppState::CurrentFile()->get_x24(ptr);
    // drawX24(&inst, pen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x28)) {
    const T28Shape<kAMax> inst = AppState::CurrentFile()->get_x28(ptr);
    DrawX28(&inst);
    // drawShape(inst->ptr5, darkerPen);
    // drawShape(inst->ptr1, darkerPen);
    // drawShape(inst->ptr2, darkerPen);
    // drawShape(inst->ptr5, darkerPen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x2D)) {
    // const T2DSymbolInstance<kAMax> inst =
    // AppState::CurrentFile()->get_x2D(ptr); drawX2D(&inst, pen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x30)) {
    // const T30StringGraphic<kAMax> &inst =
    // AppState::CurrentFile()->get_x30(ptr); drawX30(&inst, pen); } else if
    // (fs.x31_map->count(ptr) > 0) {
    //     const x31 *inst = (const x31*)&fs.x31_map->at(ptr);
    //     drawX31((const x31*)&fs.x31_map->at(ptr), pen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x32)) {
    // const T32SymbolPin<kAMax> &inst = AppState::CurrentFile()->get_x32(ptr);
    // drawX32(&inst, pen, 0);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x33)) {
    // const x33<kAMax> &inst = AppState::CurrentFile()->get_x33(ptr);
    // drawX33(&inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr1, darkerPen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x34)) {
    // const x34<kAMax> &inst = AppState::CurrentFile()->get_x34(ptr);
    // drawX34(&inst, pen);
  } else if (AppState::CurrentFile()->is_type(ptr, 0x37)) {
    // const x37<kAMax> &inst = AppState::CurrentFile()->get_x37(ptr);
    // drawShape(inst.ptr1, darkerPen);
    // for (uint8_t i; i<inst->count; i++) {
    // for (uint8_t i=0; i<inst->capacity; i++) {
    //    drawShape(inst->ptrs[i], pen);
    //}
  } else {
    // std::printf("Don't know how to draw 0x%08X\n", ntohl(ptr));
  }
}

// Implementation inspired by KiCAD, `common/gal/opengl/opengl_gal.cpp`
void BrdWidget::AddSegment(Mn::Vector2 start, Mn::Vector2 end, Mn::Half width,
                           uint8_t layer) {
  layer %= layer_colors_.size();

  lines_cache_.emplace_back(start, end, width, 0, layer);
  lines_cache_.emplace_back(start, end, width, 1, layer);
  lines_cache_.emplace_back(start, end, width, 2, layer);
  lines_cache_.emplace_back(start, end, width, 3, layer);
  lines_cache_.emplace_back(start, end, width, 4, layer);
  lines_cache_.emplace_back(start, end, width, 5, layer);

  AddLineCap(start, end, width, layer);
  AddLineCap(end, start, width, layer);
}

inline void BrdWidget::AddLineCap(Mn::Vector2 start, Mn::Vector2 end,
                                  Mn::Half width, uint8_t layer) {
  layer %= layer_colors_.size();
  lines_cache_.emplace_back(start, end, width, 6, layer);
  lines_cache_.emplace_back(start, end, width, 7, layer);
  lines_cache_.emplace_back(start, end, width, 8, layer);
}

// Modify DrawX05 to store individual segments
void BrdWidget::DrawX05(const T05Line<kAMax> *inst) {
  uint32_t k = inst->first_segment_ptr;
  uint8_t layer_id = LayerToShader(inst->layer);

  while (IsLineSegment(k)) {
    // SkPath segment_path;
    // segment_path.moveTo(starting);
    if (AppState::CurrentFile()->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x01(k);
      Mn::Half segment_width{segment_inst.width / factor_};
      AddArc(segment_inst, segment_width, layer_id);
      k = segment_inst.next;
    } else if (AppState::CurrentFile()->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x15(k);
      Mn::Half segment_width{segment_inst.width / factor_};
      DrawX15(&segment_inst, segment_width, layer_id);
      k = segment_inst.next;
    } else if (AppState::CurrentFile()->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x16(k);
      Mn::Half segment_width{segment_inst.width / factor_};
      DrawX16(&segment_inst, segment_width, layer_id);
      k = segment_inst.next;
    } else if (AppState::CurrentFile()->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x17(k);
      Mn::Half segment_width{segment_inst.width / factor_};
      DrawX17(&segment_inst, segment_width, layer_id);
      k = segment_inst.next;
    } else {
      return;
    }
  }
}

void BrdWidget::DrawX15(const T15LineSegment<kAMax> *inst, Mn::Half width,
                        uint8_t layer_id) {
  Mn::Vector2 start = {inst->coords[0] / factor_, inst->coords[1] / factor_};
  Mn::Vector2 end = {inst->coords[2] / factor_, inst->coords[3] / factor_};
  AddSegment(start, end, width, layer_id);
}

void BrdWidget::DrawX16(const T16LineSegment<kAMax> *inst, Mn::Half width,
                        uint8_t layer_id) {
  Mn::Vector2 start = {inst->coords[0] / factor_, inst->coords[1] / factor_};
  Mn::Vector2 end = {inst->coords[2] / factor_, inst->coords[3] / factor_};
  AddSegment(start, end, width, layer_id);
}

void BrdWidget::DrawX17(const T17LineSegment<kAMax> *inst, Mn::Half width,
                        uint8_t layer_id) {
  Mn::Vector2 start = {inst->coords[0] / factor_, inst->coords[1] / factor_};
  Mn::Vector2 end = {inst->coords[2] / factor_, inst->coords[3] / factor_};
  AddSegment(start, end, width, layer_id);
}

void BrdWidget::DrawX28(const T28Shape<kAMax> *inst) {
  uint32_t k = inst->first_segment_ptr;
  uint8_t layer_id = LayerToShader(inst->layer);

  while (IsLineSegment(k)) {
    if (AppState::CurrentFile()->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x01(k);
      AddArc(segment_inst, border_width_, layer_id);
      k = segment_inst.next;
    } else if (AppState::CurrentFile()->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x15(k);
      DrawX15(&segment_inst, border_width_, layer_id);
      k = segment_inst.next;
    } else if (AppState::CurrentFile()->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x16(k);
      DrawX16(&segment_inst, border_width_, layer_id);
      k = segment_inst.next;
    } else if (AppState::CurrentFile()->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst =
          AppState::CurrentFile()->get_x17(k);
      DrawX17(&segment_inst, border_width_, layer_id);
      k = segment_inst.next;
    } else {
      return;
    }
  }
}

auto BrdWidget::StartingPoint(uint32_t k) -> std::optional<Mn::Vector2> {
  if (AppState::CurrentFile()->is_type(k, 0x01)) {
    const T01ArcSegment<kAMax> segment_inst =
        AppState::CurrentFile()->get_x01(k);
    return Mn::Vector2({static_cast<float>(segment_inst.coords[0]),
                        static_cast<float>(segment_inst.coords[1])});
  }
  if (AppState::CurrentFile()->is_type(k, 0x15)) {
    const T15LineSegment<kAMax> segment_inst =
        AppState::CurrentFile()->get_x15(k);
    return Mn::Vector2({static_cast<float>(segment_inst.coords[0]),
                        static_cast<float>(segment_inst.coords[1])});
  }
  if (AppState::CurrentFile()->is_type(k, 0x16)) {
    const T16LineSegment<kAMax> segment_inst =
        AppState::CurrentFile()->get_x16(k);
    return Mn::Vector2({static_cast<float>(segment_inst.coords[0]),
                        static_cast<float>(segment_inst.coords[1])});
  }
  if (AppState::CurrentFile()->is_type(k, 0x17)) {
    const T17LineSegment<kAMax> segment_inst =
        AppState::CurrentFile()->get_x17(k);
    return Mn::Vector2({static_cast<float>(segment_inst.coords[0]),
                        static_cast<float>(segment_inst.coords[1])});
  }
  return {};
}

auto BrdWidget::IsLineSegment(uint32_t k) -> bool {
  bool r = (AppState::CurrentFile()->is_type(k, 0x01)) ||
           (AppState::CurrentFile()->is_type(k, 0x15)) ||
           (AppState::CurrentFile()->is_type(k, 0x16)) ||
           (AppState::CurrentFile()->is_type(k, 0x17));
  // std::printf("isLineSegment k = 0x%08X, r = %d\n", ntohl(k), r);
  return r;
}

auto BrdWidget::ScreenToWorld(const Mn::Vector2 &screen_pos, bool center)
    -> Mn::Vector2 {
  // FIXME: Cache?
  auto viewportSize = Mn::GL::defaultFramebuffer.viewport();

  // FIXME: Why 4?
  float offset = center ? 1.0F : 0.0F;
  Mn::Vector2 normalized =
      screen_pos / Mn::Vector2(viewportSize.sizeX() / 4.0F,
                               viewportSize.sizeY() / 4.0F) -
      Mn::Vector2{offset, offset};

  normalized.y() = -normalized.y();
  Mn::Matrix3 complete_transform = projection_matrix_ * aspect_ratio_matrix_;
  if (center) {
    complete_transform *transformation_matrix_;
  }
  return complete_transform.inverted().transformPoint(normalized);
}

auto BrdWidget::LayerToShader(const LayerInfo layer) -> uint8_t {
  if (layer.family == 6 && layer.id <= 7) {
    return layer.id;
  }
  return 8;
}

// Add method to check if point is near path
/*
auto BrdWidget::IsPointNearPath(const SkPath &path, const SkPoint &point,
                                float width) -> bool {
  SkPath stroke_path;

  // Create stroke path using SkStrokeRec
  SkStrokeRec stroke(SkStrokeRec::kFill_InitStyle);
  stroke.setStrokeStyle(width);
  stroke.setStrokeParams(SkPaint::kRound_Cap, SkPaint::kRound_Join, 4.0F);

  // Generate the stroked path
  SkPath result;
  stroke.applyToPath(&result, path);

  return result.contains(point.x(), point.y());
}
*/

void BrdWidget::HandleMouseWheel(
    Mn::Platform::EmscriptenApplication::ScrollEvent &event) {
  event.setAccepted(true);

  constexpr float zoom_factor = 1.05F;
  float wheel_y = -event.event().deltaY;

  if (wheel_y == 0) {
    return;
  }

  Mn::Vector2 mouse_screen_pos = event.position();
  Mn::Vector2 mouse_world_pos = ScreenToWorld(mouse_screen_pos, true);

  // Compute zoom scale factor
  float scale = (wheel_y < 0) ? 1.0F / zoom_factor : zoom_factor;

  // Apply zoom limits
  float current_scale = projection_matrix_.scaling().x();
  if ((wheel_y < 0 && current_scale > 0.01F) ||
      (wheel_y > 0 && current_scale < 20.0F)) {
    // Apply scaling to the projection matrix
    projection_matrix_ =
        Mn::Matrix3::scaling(Mn::Vector2(scale)) * projection_matrix_;

    Mn::Vector2 new_mouse_world_pos = ScreenToWorld(mouse_screen_pos, true);

    Mn::Vector2 correction = new_mouse_world_pos - mouse_world_pos;
    transformation_matrix_ =
        Mn::Matrix3::translation(correction) * transformation_matrix_;
  }

  dirty_ = true;
}

void BrdWidget::HandleMouseDown(
    Mn::Platform::EmscriptenApplication::PointerEvent &event) {
  if (event.pointer() ==
      Mn::Platform::EmscriptenApplication::Pointer::MouseLeft) {
    is_panning_ = true;
    last_mouse_pos_ = event.position();
  }
}

void BrdWidget::HandleMouseUp(
    Mn::Platform::EmscriptenApplication::PointerEvent &event) {
  if (event.pointer() ==
      Mn::Platform::EmscriptenApplication::Pointer::MouseLeft) {
    is_panning_ = false;
  }
}

void BrdWidget::HandleMouseMove(
    Mn::Platform::EmscriptenApplication::PointerMoveEvent &event) {
  if (is_panning_) {
    Mn::Vector2 current_pos = event.position();
    Mn::Vector2 delta = current_pos - last_mouse_pos_;

    Mn::Vector2 world_delta = ScreenToWorld(delta);

    transformation_matrix_ =
        Mn::Matrix3::translation(world_delta) * transformation_matrix_;

    last_mouse_pos_ = current_pos;

    dirty_ = true;
  }
}

void BrdWidget::UpdateScreenRatio() {
  auto viewportSize = Mn::GL::defaultFramebuffer.viewport().size();
  float aspect = 1.0F * viewportSize.x() / viewportSize.y();

  // Adjust scaling to maintain aspect ratio
  aspect_ratio_matrix_ =
      Mn::Matrix3::scaling({1, static_cast<float>(1. * aspect)});
}
