#include "webapp/brd_renderer.h"

#include <Corrade/Containers/ArrayView.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/Math/Color.h>
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

#include <cmath>

#include "Magnum/GL/DefaultFramebuffer.h"
#include "Magnum/Primitives/Line.h"
#include "emscripten_browser_file.h"
#include "lib/structure/utils.h"
#include "webapp/app_state.h"

using namespace Magnum::Math::Literals;

BrdWidget::BrdWidget() {
  InitializeShader();
  UpdateScreenRatio();

  triangle_mesh_.setCount(0);
  mesh_.setCount(0);
}

void BrdWidget::UpdateFile() {
  fs_ = AppState::CurrentFile();

  already_drawn_.clear();
  lines_cache_.clear();

  IterateFile();
  ComposeLayersToDrawable();

  struct TriangleVertex {
    Magnum::Vector2 position;
    Magnum::Color3 color;
  };
  const TriangleVertex vertices[3]{
      {{-0.5F, -0.5F}, 0xff0000_rgbf}, /* Left vertex, red color */
      {{0.5F, -0.5F}, 0x00ff00_rgbf},  /* Right vertex, green color */
      {{0.0F, 0.5F}, 0x0000ff_rgbf}    /* Top vertex, blue color */
  };

  triangle_mesh_.setCount(Corrade::Containers::arraySize(vertices))
      .addVertexBuffer(Magnum::GL::Buffer{vertices}, 0,
                       Magnum::Shaders::VertexColorGL2D::Position{},
                       Magnum::Shaders::VertexColorGL2D::Color3{});

  // Magnum::Trade::MeshData circle = Magnum::Primitives::circle2DWireframe(16);
  mesh_ = Magnum::MeshTools::compileLines(Magnum::MeshTools::generateLines(
      Magnum::MeshTools::concatenate(Corrade::Containers::arrayView(
          lines_cache_.data(), lines_cache_.size()))));
  emscripten_log(EM_LOG_INFO, "mesh_ count=%d", mesh_.count());

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

void BrdWidget::UpdateLayerAlpha(uint8_t /* unused */, float /* unused */) {}

void BrdWidget::Draw() {
  shader_.setViewportSize(Magnum::Vector2{300, 300})
      .setTransformationProjectionMatrix(
          projection_matrix_ * aspect_ratio_matrix_ * transformation_matrix_)
      .setColor(0x2f83cc_rgbf)
      .setWidth(1.F)
      .draw(mesh_);
  triangle_shader_.draw(triangle_mesh_);

  // const auto &visible_layers = AppState::VisibleLayers();

  /*
  SkCanvas *canvas = surface->getCanvas();
  cached_height_ = surface->height();

  bool updated = (visible_layers_cache_ != visible_layers);
  if (updated || AppState::PictureNeedsRecording()) {
    emscripten_log(EM_LOG_INFO, "updated!");
    std::array<bool, 10> selected = {false};
    for (LayerInfo const &visible_layer : visible_layers) {
      selected[LayerToShader(visible_layer)] = true;
    }
    for (uint8_t i = 0; i < 9; i++) {
      UpdateLayerAlpha(i, selected[i] ? 1.0F : 0.05F);
    }
    visible_layers_cache_ = visible_layers;

    ComposeLayersToDrawable();
    dirty_ = true;
  }

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

  for (const auto &i_x1B : fs_->iter_t1B_net()) {
    for (const auto &i_x04 : fs_->iter_x04(i_x1B.k)) {
      uint32_t k = i_x04.ptr2;
      while (true) {
        if (fs_->is_type(k, 0x33)) {
          const auto &i = fs_->get_x33(k);
          // printf("- - Found x33 w/ key = 0x %08X\n", ntohl(k));
          // drawShape(k, pen3);
          k = i.un1;
        } else if (fs_->is_type(k, 0x32)) {
          const auto &i = fs_->get_x32(k);
          // printf("- - Found x32 w/ key = 0x %08X\n", ntohl(k));
          k = i.un1;
        } else if (fs_->is_type(k, 0x2E)) {
          const auto &i = fs_->get_x2E(k);
          // printf("- - Found x2E w/ key = 0x %08X\n", ntohl(k));
          k = i.un[0];
        } else if (fs_->is_type(k, 0x28)) {
          const auto &x = fs_->get_x28(k);
          DrawShape(x.k);
          k = x.next;
        } else if (fs_->is_type(k, 0x1B)) {
          break;
        } else if (fs_->is_type(k, 0x0E)) {
          const auto &x = fs_->get_x0E(k);
          k = x.un[0];
        } else if (fs_->is_type(k, 0x05)) {
          const auto &x = fs_->get_x05(k);
          DrawShape(x.k);
          k = x.ptr0;
        } else if (fs_->is_type(k, 0x04)) {
          break;
        } else {
          printf("- - \x1b[31mUnexpected key\x1b[0m = 0x %08X :(\n", ntohl(k));
          break;
        }
      }
    }
  }
}

void BrdWidget::DrawShape(uint32_t ptr) {
  if (already_drawn_.count(ptr) > 0) {
    return;
  }

  already_drawn_.insert(ptr);

  if (fs_->is_type(ptr, 0x05)) {
    // std::printf("Trying to draw x05\n");
    const T05Line<kAMax> inst = fs_->get_x05(ptr);
    DrawX05(&inst);
  } else if (fs_->is_type(ptr, 0x0C)) {
    // const T0CDrillIndicator<kAMax> inst = fs_->get_x0C(ptr);
    // drawX0C(&inst, pen);
  } else if (fs_->is_type(ptr, 0x10)) {
    // const x10<kAMax> inst = fs_->get_x10(ptr);
    // drawShape(inst.ptr1, darkerPen);
    // drawShape(inst.ptr2, darkerPen);
    // drawShape(inst.ptr3, darkerPen);
  } else if (fs_->is_type(ptr, 0x14)) {
    // const T14Path<kAMax> inst = fs_->get_x14(ptr);
    // drawX14(&inst, pen);
    // } else if (fs_->x15_map.count(ptr) > 0) {
    // const x15<A_MAX> *inst = (const x15<A_MAX> *)&fs_->x15_map.at(ptr);
    // drawX15(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
    // } else if (fs_->x16_map.count(ptr) > 0) {
    // const x16<A_MAX> *inst = (const x16<A_MAX> *)&fs_->x16_map.at(ptr);
    // drawX16(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
    // } else if (fs_->x17_map.count(ptr) > 0) {
    // const x17<A_MAX> *inst = (const x17<A_MAX> *)&fs_->x17_map.at(ptr);
    // drawX17(inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr, darkerPen);
  } else if (fs_->is_type(ptr, 0x23)) {
    // const T23Rat<kAMax> inst = fs_->get_x23(ptr);
    // drawX23(&inst, pen);
  } else if (fs_->is_type(ptr, 0x24)) {
    // const T24Rectangle<kAMax> inst = fs_->get_x24(ptr);
    // drawX24(&inst, pen);
  } else if (fs_->is_type(ptr, 0x28)) {
    const T28Shape<kAMax> inst = fs_->get_x28(ptr);
    DrawX28(&inst);
    // drawShape(inst->ptr5, darkerPen);
    // drawShape(inst->ptr1, darkerPen);
    // drawShape(inst->ptr2, darkerPen);
    // drawShape(inst->ptr5, darkerPen);
  } else if (fs_->is_type(ptr, 0x2D)) {
    // const T2DSymbolInstance<kAMax> inst = fs_->get_x2D(ptr);
    // drawX2D(&inst, pen);
  } else if (fs_->is_type(ptr, 0x30)) {
    // const T30StringGraphic<kAMax> &inst = fs_->get_x30(ptr);
    // drawX30(&inst, pen);
    // } else if (fs.x31_map->count(ptr) > 0) {
    //     const x31 *inst = (const x31*)&fs.x31_map->at(ptr);
    //     drawX31((const x31*)&fs.x31_map->at(ptr), pen);
  } else if (fs_->is_type(ptr, 0x32)) {
    // const T32SymbolPin<kAMax> &inst = fs_->get_x32(ptr);
    // drawX32(&inst, pen, 0);
  } else if (fs_->is_type(ptr, 0x33)) {
    // const x33<kAMax> &inst = fs_->get_x33(ptr);
    // drawX33(&inst, pen);
    // drawShape(inst->un1, darkerPen);
    // drawShape(inst->ptr1, darkerPen);
  } else if (fs_->is_type(ptr, 0x34)) {
    // const x34<kAMax> &inst = fs_->get_x34(ptr);
    // drawX34(&inst, pen);
  } else if (fs_->is_type(ptr, 0x37)) {
    // const x37<kAMax> &inst = fs_->get_x37(ptr);
    // drawShape(inst.ptr1, darkerPen);
    // for (uint8_t i; i<inst->count; i++) {
    // for (uint8_t i=0; i<inst->capacity; i++) {
    //    drawShape(inst->ptrs[i], pen);
    //}
  } else {
    // std::printf("Don't know how to draw 0x%08X\n", ntohl(ptr));
  }
}

// Modify DrawX05 to store individual segments
void BrdWidget::DrawX05(const T05Line<kAMax> *inst) {
  uint32_t k = inst->first_segment_ptr;
  // uint8_t layer_id = LayerToShader(inst->layer);

  Magnum::Vector2 starting = (*StartingPoint(k)) * (1.0 / factor_);

  while (IsLineSegment(k)) {
    // SkPath segment_path;
    // segment_path.moveTo(starting);
    Magnum::Vector2 next;
    // float segment_width;

    if (fs_->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst = fs_->get_x01(k);
      // segment_width = segment_inst.width / factor_;

      /*
      auto [cx, cy] = x01_center(&segment_inst);
      float scaled_cx = cx / factor_;
      float scaled_cy = cy / factor_;
      float scaled_r = (static_cast<double>(segment_inst.r)) / factor_;

      Magnum::Vector2 start{segment_inst.coords[0] / factor_,
                    segment_inst.coords[1] / factor_};
      Magnum::Vector2 end{segment_inst.coords[2] / factor_,
                  segment_inst.coords[3] / factor_};

      float start_angle =
          SkRadiansToDegrees(atan2(start.fY - scaled_cy, start.fX - scaled_cx));
      float end_angle =
          SkRadiansToDegrees(atan2(end.fY - scaled_cy, end.fX - scaled_cx));

      float sweep_angle = NAN;
      if (segment_inst.subtype != 0) {
        sweep_angle = end_angle - start_angle;
        if (sweep_angle > 0) {
          sweep_angle -= 360;
        }
      } else {
        sweep_angle = end_angle - start_angle;
        if (sweep_angle < 0) {
          sweep_angle += 360;
        }
      }

      SkRect oval_bounds =
          SkRect::MakeXYWH(scaled_cx - scaled_r, scaled_cy - scaled_r,
                           scaled_r * 2, scaled_r * 2);

      segment_path.arcTo(oval_bounds, start_angle, sweep_angle, false);
      next = end;
      */

      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst = fs_->get_x15(k);
      // segment_width = segment_inst.width / factor_;
      next = Magnum::Vector2(
          {segment_inst.coords[2] / factor_, segment_inst.coords[3] / factor_});
      // segment_path.lineTo(next);
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst = fs_->get_x16(k);
      // segment_width = segment_inst.width / factor_;
      next = Magnum::Vector2(
          {segment_inst.coords[2] / factor_, segment_inst.coords[3] / factor_});
      // segment_path.lineTo(next);
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst = fs_->get_x17(k);
      // segment_width = segment_inst.width / factor_;
      next = Magnum::Vector2(
          {segment_inst.coords[2] / factor_, segment_inst.coords[3] / factor_});
      // segment_path.lineTo(next);
      k = segment_inst.next;
    } else {
      return;
    }

    // Store segment info
    emscripten_log(EM_LOG_INFO, "adding line {%f %f}, {%f %f}", starting.x(),
                   starting.y(), next.x(), next.y());
    lines_cache_.push_back(Magnum::Primitives::line2D(starting, next));
    // segment_paths_.push_back(
    //     {segment_path, segment_width, layer_id, inst->layer});

    // Add to layer paths as before
    /*
    size_t width_index = GetWidthIndex(segment_width);
    if (width_index < common_width_count_) {
      shader_layers_[layer_id].common_width_paths[width_index].addPath(
          segment_path);
    } else {
      auto &other_paths = shader_layers_[layer_id].other_width_paths;
      auto it =
          std::find_if(other_paths.begin(), other_paths.end(),
                       [segment_width](const auto &pair) {
                         return std::abs(pair.first - segment_width) < 0.001F;
                       });
      if (it != other_paths.end()) {
        it->second.addPath(segment_path);
      } else {
        other_paths.emplace_back(segment_width, segment_path);
      }
      */

    starting = next;
  }
}

void BrdWidget::DrawX28(const T28Shape<kAMax> *inst) {
  uint32_t k = inst->first_segment_ptr;
  // uint8_t layer_id = LayerToShader(inst->layer);

  // float current_width = -1;
  // size_t width_index = common_width_count_;

  // SkPath current_path;
  Magnum::Vector2 starting = (*StartingPoint(k)) * (1.0 / factor_);
  Magnum::Vector2 first_point = starting;
  // current_path.moveTo(starting);

  while (IsLineSegment(k)) {
    // Magnum::Vector2 next;
    break;
    /*
    float segment_width;

    if (fs_->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst = fs_->get_x01(k);
      segment_width = segment_inst.width / factor_;

      // Calculate center and angles
      auto [cx, cy] = x01_center(&segment_inst);

      // Convert to drawing coordinates
      float scaled_cx = cx / factor_;
      float scaled_cy = cy / factor_;
      float scaled_r = (static_cast<double>(segment_inst.r)) / factor_;

      // Calculate start and sweep angles
      SkPoint start{segment_inst.coords[0] / factor_,
                    segment_inst.coords[1] / factor_};
      SkPoint end{segment_inst.coords[2] / factor_,
                  segment_inst.coords[3] / factor_};

      float start_angle =
          SkRadiansToDegrees(atan2(start.fY - scaled_cy, start.fX - scaled_cx));
      float end_angle =
          SkRadiansToDegrees(atan2(end.fY - scaled_cy, end.fX - scaled_cx));

      float sweep_angle = NAN;
      if (segment_inst.subtype != 0) {
        sweep_angle = end_angle - start_angle;
        if (sweep_angle > 0) {
          sweep_angle -= 360;
        }
      } else {
        sweep_angle = end_angle - start_angle;
        if (sweep_angle < 0) {
          sweep_angle += 360;
        }
      }

      // Draw the arc
      SkRect oval_bounds =
          SkRect::MakeXYWH(scaled_cx - scaled_r, scaled_cy - scaled_r,
                           scaled_r * 2, scaled_r * 2);

      current_path.arcTo(oval_bounds, start_angle, sweep_angle, false);

      next = end;
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst = fs_->get_x15(k);
      segment_width = segment_inst.width / factor_;
      next = SkPoint::Make(segment_inst.coords[2] / factor_,
                           segment_inst.coords[3] / factor_);
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst = fs_->get_x16(k);
      segment_width = segment_inst.width / factor_;
      next = SkPoint::Make(segment_inst.coords[2] / factor_,
                           segment_inst.coords[3] / factor_);
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst = fs_->get_x17(k);
      segment_width = segment_inst.width / factor_;
      next = SkPoint::Make(segment_inst.coords[2] / factor_,
                           segment_inst.coords[3] / factor_);
      k = segment_inst.next;
    } else {
      return;
    }

    if (segment_width != current_width) {
      // If width changes, store current path
      if (!current_path.isEmpty()) {
        if (width_index < common_width_count_) {
          shader_layers_[layer_id].common_width_paths[width_index].addPath(
              current_path);
        } else {
          // Try to find an existing path with this width
          auto &other_paths = shader_layers_[layer_id].other_width_paths;
          auto it = std::find_if(other_paths.begin(), other_paths.end(),
                                 [current_width](const auto &pair) {
                                   return std::abs(pair.first - current_width) <
                                          0.001F;
                                 });
          if (it != other_paths.end()) {
            it->second.addPath(current_path);
          } else {
            other_paths.emplace_back(current_width, current_path);
          }
        }
        current_path.reset();
        current_path.moveTo(starting);
      }
      current_width = segment_width;
      width_index = GetWidthIndex(segment_width);
    }

    current_path.lineTo(next);
    starting = next;
    */
  }

  // Ensure the path is properly closed by explicitly returning to start
  if (starting != first_point) {
    // current_path.lineTo(first_point);
  }
  // current_path.close();

  // Force path direction to be consistent
  // current_path.setFillType(SkPathFillType::kWinding);

  // shader_layers_[layer_id].filled_path.addPath(current_path);
}

auto BrdWidget::StartingPoint(uint32_t k) -> std::optional<Magnum::Vector2> {
  if (fs_->is_type(k, 0x01)) {
    const T01ArcSegment<kAMax> segment_inst = fs_->get_x01(k);
    return Magnum::Vector2({static_cast<float>(segment_inst.coords[0]),
                            static_cast<float>(segment_inst.coords[1])});
  }
  if (fs_->is_type(k, 0x15)) {
    const T15LineSegment<kAMax> segment_inst = fs_->get_x15(k);
    return Magnum::Vector2({static_cast<float>(segment_inst.coords[0]),
                            static_cast<float>(segment_inst.coords[1])});
  }
  if (fs_->is_type(k, 0x16)) {
    const T16LineSegment<kAMax> segment_inst = fs_->get_x16(k);
    return Magnum::Vector2({static_cast<float>(segment_inst.coords[0]),
                            static_cast<float>(segment_inst.coords[1])});
  }
  if (fs_->is_type(k, 0x17)) {
    const T17LineSegment<kAMax> segment_inst = fs_->get_x17(k);
    return Magnum::Vector2({static_cast<float>(segment_inst.coords[0]),
                            static_cast<float>(segment_inst.coords[1])});
  }
  return {};
}

auto BrdWidget::IsLineSegment(uint32_t k) -> bool {
  bool r = (fs_->is_type(k, 0x01)) || (fs_->is_type(k, 0x15)) ||
           (fs_->is_type(k, 0x16)) || (fs_->is_type(k, 0x17));
  // std::printf("isLineSegment k = 0x%08X, r = %d\n", ntohl(k), r);
  return r;
}

auto BrdWidget::GetWidthIndex(float /* unused */) -> size_t {
  return 0;
  /*
  // First check common widths
  for (size_t i = 0; i < common_width_count_; i++) {
    if (std::abs(common_widths_[i] - width) < 0.001F) {
      return i;
    }
    // If we find an uninitialized slot, use it
    if (common_widths_[i] == 0.0F) {
      common_widths_[i] = width;
      return i;
    }
  }
  return common_width_count_;  // Indicate this needs to go in other_width_paths
  */
}

auto BrdWidget::ScreenToWorld(const Magnum::Vector2 &screen_pos, bool center)
    -> Magnum::Vector2 {
  // FIXME: Cache?
  auto viewportSize = Magnum::GL::defaultFramebuffer.viewport();

  // FIXME: Why 4?
  float offset = center ? 1.0F : 0.0F;
  Magnum::Vector2 normalized =
      screen_pos / Magnum::Vector2(viewportSize.sizeX() / 4.0F,
                                   viewportSize.sizeY() / 4.0F) -
      Magnum::Vector2{offset, offset};

  normalized.y() = -normalized.y();
  Magnum::Matrix3 complete_transform =
      projection_matrix_ * aspect_ratio_matrix_;
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
    Magnum::Platform::EmscriptenApplication::ScrollEvent &event) {
  event.setAccepted(true);

  constexpr float zoom_factor = 1.05F;
  float wheel_y = event.event().deltaY;

  if (wheel_y == 0) {
    return;
  }

  Magnum::Vector2 mouse_screen_pos = event.position();
  Magnum::Vector2 mouse_world_pos = ScreenToWorld(mouse_screen_pos, true);

  // Compute zoom scale factor
  float scale = (wheel_y < 0) ? 1.0F / zoom_factor : zoom_factor;

  // Apply zoom limits
  float current_scale = projection_matrix_.scaling().x();
  if ((wheel_y < 0 && current_scale > 0.01F) ||
      (wheel_y > 0 && current_scale < 20.0F)) {
    // Apply scaling to the projection matrix
    projection_matrix_ =
        Magnum::Matrix3::scaling(Magnum::Vector2(scale)) * projection_matrix_;

    Magnum::Vector2 new_mouse_world_pos = ScreenToWorld(mouse_screen_pos, true);

    Magnum::Vector2 correction = new_mouse_world_pos - mouse_world_pos;
    transformation_matrix_ =
        Magnum::Matrix3::translation(correction) * transformation_matrix_;
  }

  dirty_ = true;
}

void BrdWidget::HandleMouseDown(
    Magnum::Platform::EmscriptenApplication::PointerEvent &event) {
  if (event.pointer() ==
      Magnum::Platform::EmscriptenApplication::Pointer::MouseLeft) {
    is_panning_ = true;
    last_mouse_pos_ = event.position();
  }
}

void BrdWidget::HandleMouseUp(
    Magnum::Platform::EmscriptenApplication::PointerEvent &event) {
  if (event.pointer() ==
      Magnum::Platform::EmscriptenApplication::Pointer::MouseLeft) {
    is_panning_ = false;
  }
}

void BrdWidget::HandleMouseMove(
    Magnum::Platform::EmscriptenApplication::PointerMoveEvent &event) {
  if (is_panning_) {
    Magnum::Vector2 current_pos = event.position();
    Magnum::Vector2 delta = current_pos - last_mouse_pos_;

    Magnum::Vector2 world_delta = ScreenToWorld(delta);

    transformation_matrix_ =
        Magnum::Matrix3::translation(world_delta) * transformation_matrix_;

    last_mouse_pos_ = current_pos;

    dirty_ = true;
  }
}

void BrdWidget::UpdateScreenRatio() {
  auto viewportSize = Magnum::GL::defaultFramebuffer.viewport().size();
  float aspect = viewportSize.x() / viewportSize.y();

  // Adjust scaling to maintain aspect ratio
  aspect_ratio_matrix_ =
      Magnum::Matrix3::scaling({1, static_cast<float>(1. * aspect)});
}
