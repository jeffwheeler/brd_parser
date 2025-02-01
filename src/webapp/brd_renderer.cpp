#include "webapp/brd_renderer.h"

#include <SDL_events.h>
#include <SDL_mouse.h>
#include <arpa/inet.h>
#include <emscripten.h>
#include <math.h>

#include "include/core/SkRect.h"
#include "include/core/SkStrokeRec.h"
#include "include/pathops/SkPathOps.h"
#include "lib/structure/utils.h"

BrdWidget::BrdWidget() : fs_(nullptr) { InitializeShader(); }

void BrdWidget::UpdateFile(std::shared_ptr<File<kAMax>> fs) {
  fs_ = std::move(fs);
  segment_paths_.clear();
  hover_segment_index_ = -1;
  already_drawn_.clear();
  for (auto &layer_path : layer_paths_) {
    for (uint8_t j = 0; j < common_width_count_; j++) {
      layer_path.common_width_paths[j].reset();
    }
    layer_path.other_width_paths.clear();
    layer_path.filled_path.reset();
  }

  IterateFile();
  ComposeLayersToDrawable();

  dirty_ = true;
}

void BrdWidget::ComposeLayersToDrawable() {
  SkPictureRecorder recorder;

  SkRect bbox = {-10000, -10000, 10000, 10000};
  recorder.beginRecording(bbox, nullptr);

  for (uint8_t i = 0; i < 9; i++) {
    // Base paint settings for strokes
    SkPaint stroke_paint;
    stroke_paint.setStyle(SkPaint::kStroke_Style);
    stroke_paint.setShader(layer_shaders_[i]);
    stroke_paint.setAntiAlias(true);
    stroke_paint.setStrokeCap(SkPaint::kRound_Cap);
    stroke_paint.setStrokeJoin(SkPaint::kRound_Join);
    stroke_paint.setBlendMode(SkBlendMode::kLighten);

    // Draw stroked paths
    for (size_t w = 0; w < common_width_count_; w++) {
      if (!layer_paths_[i].common_width_paths[w].isEmpty()) {
        stroke_paint.setStrokeWidth(common_widths_[w]);
        recorder.getRecordingCanvas()->drawPath(
            layer_paths_[i].common_width_paths[w], stroke_paint);
      }
    }

    for (const auto &[width, path] : layer_paths_[i].other_width_paths) {
      stroke_paint.setStrokeWidth(width);
      recorder.getRecordingCanvas()->drawPath(path, stroke_paint);
    }

    if (!layer_paths_[i].filled_path.isEmpty()) {
      SkPaint fill_paint;
      fill_paint.setStyle(SkPaint::kFill_Style);
      fill_paint.setShader(layer_shaders_[i]);
      fill_paint.setAntiAlias(true);
      fill_paint.setBlendMode(SkBlendMode::kLighten);
      recorder.getRecordingCanvas()->drawPath(layer_paths_[i].filled_path,
                                              fill_paint);
    }
  }

  picture_ = recorder.finishRecordingAsPicture();
}
void BrdWidget::InitializeShader() {
  const char *shader_source = R"(
    uniform half4 base_color;
    uniform float opacity;  // Not used anymore

    half4 main(vec2 coord) {
      return half4(base_color.rgb * base_color.a, base_color.a);
    }
  )";

  auto [effect, error] =
      SkRuntimeEffect::MakeForShader(SkString(shader_source));
  if (!effect) {
    printf("Shader compilation failed: %s\n", error.c_str());
    return;
  }
  runtime_effect_ = effect;

  // Initialize default colors
  layer_colors_[0] = SkColor4f::FromColor(SkColorSetARGB(255, 128, 0, 255));
  layer_colors_[1] = SkColor4f::FromColor(SkColorSetARGB(255, 180, 62, 143));
  layer_colors_[2] = SkColor4f::FromColor(SkColorSetARGB(255, 160, 175, 132));
  layer_colors_[3] = SkColor4f::FromColor(SkColorSetARGB(255, 255, 237, 223));
  layer_colors_[4] = SkColor4f::FromColor(SkColorSetARGB(255, 197, 216, 109));
  layer_colors_[5] = SkColor4f::FromColor(SkColorSetARGB(255, 134, 97, 92));
  layer_colors_[6] = SkColor4f::FromColor(SkColorSetARGB(255, 175, 224, 206));
  layer_colors_[7] = SkColor4f::FromColor(SkColorSetARGB(255, 244, 116, 59));
  layer_colors_[8] = SkColor4f::FromColor(SkColorSetARGB(255, 59, 0, 134));

  UpdateLayerShaders();
}

void BrdWidget::UpdateLayerShaders() {
  for (size_t i = 0; i < layer_colors_.size(); i++) {
    SkRuntimeShaderBuilder builder(runtime_effect_);
    builder.uniform("base_color") = layer_colors_[i];
    builder.uniform("opacity") = 1.0F;
    layer_shaders_[i] = builder.makeShader();
  }
}

void BrdWidget::UpdateLayerAlpha(uint8_t layer, float new_alpha) {
  if (layer < layer_colors_.size()) {
    layer_colors_[layer].fA = new_alpha;
    SkRuntimeShaderBuilder builder(runtime_effect_);
    builder.uniform("base_color") = layer_colors_[layer];
    builder.uniform("opacity") = 1.0F;
    // No need to set opacity uniform if using first shader version
    layer_shaders_[layer] = builder.makeShader();
  }
}

void BrdWidget::Draw(SkSurface *surface, double device_pixel_ratio_,
                     std::array<bool, 10> &selected_layers) {
  SkCanvas *canvas = surface->getCanvas();
  cached_height_ = surface->height();

  bool updated = false;
  for (uint8_t i = 0; i < 9; i++) {
    UpdateLayerAlpha(i, selected_layers[i] ? 1.0F : 0.05F);
    if (selected_layers_cache_[i] != selected_layers[i]) {
      selected_layers_cache_[i] = selected_layers[i];
      updated = true;
    }
  }
  if (updated) {
    ComposeLayersToDrawable();
    dirty_ = true;
  }

  if (!dirty_) {
    return;
  }

  dirty_ = false;

  canvas->clear(SkColorSetARGB(255, 30, 30, 30));
  canvas->save();
  canvas->translate(pan_.x() * zoom_, pan_.y() * zoom_);
  canvas->scale(zoom_, zoom_);

  // Draw base picture
  canvas->drawPicture(picture_.get());

  // Draw hover effect if applicable
  if (hover_segment_index_ >= 0) {
    const auto &segment = segment_paths_[hover_segment_index_];
    SkPaint hover_paint;
    hover_paint.setStyle(SkPaint::kStroke_Style);
    hover_paint.setColor(SkColorSetARGB(255, 255, 255, 0));  // Yellow highlight
    hover_paint.setStrokeWidth(segment.width *
                               1.2F);  // Slightly wider than original
    hover_paint.setAntiAlias(true);
    hover_paint.setStrokeCap(SkPaint::kRound_Cap);
    hover_paint.setStrokeJoin(SkPaint::kRound_Join);
    hover_paint.setBlendMode(SkBlendMode::kPlus);

    canvas->drawPath(segment.path, hover_paint);
  }

  canvas->restore();
}

void BrdWidget::HandleMouseWheel(const SDL_Event &event) {
  const float zoom_factor = 1.1F;
  // Get mouse position
  int mouseX = 0;
  int mouseY = 0;
  SDL_GetMouseState(&mouseX, &mouseY);

  // Convert mouse position to world space before zoom
  SkPoint mouse_world =
      ScreenToWorld(SkPoint::Make(mouseX, (cached_height_ / 2.0) - mouseY));

  // Adjust zoom based on wheel direction
  // Use preciseY for high-resolution scroll wheels
  float wheel_y = event.wheel.preciseY;
  if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
    wheel_y = -wheel_y;
  }

  if (wheel_y > 0) {
    zoom_ *= pow(zoom_factor, wheel_y);
  } else if (wheel_y < 0) {
    zoom_ *= pow(1.0F / zoom_factor, -wheel_y);
  }

  // Clamp zoom to reasonable limits
  zoom_ = std::max(0.1F, std::min(zoom_, 20.0F));

  // Adjust pan to keep mouse position fixed
  SkPoint new_mouse_world =
      ScreenToWorld(SkPoint::Make(mouseX, (cached_height_ / 2.0) - mouseY));
  pan_ += new_mouse_world - mouse_world;

  dirty_ = true;
}

void BrdWidget::HandleMouseDown(const SDL_Event &event) {
  if (event.button.button == SDL_BUTTON_LEFT) {
    is_panning_ = true;
    last_mouse_pos_ = SkPoint::Make(event.button.x, event.button.y);
  }
}

void BrdWidget::HandleMouseUp(const SDL_Event &event) {
  if (event.button.button == SDL_BUTTON_LEFT) {
    is_panning_ = false;
  }
}

// Modify HandleMouseMove to detect hover
void BrdWidget::HandleMouseMove(const SDL_Event &event) {
  if (is_panning_) {
    SkPoint current_pos{static_cast<float>(event.motion.x),
                        static_cast<float>(event.motion.y)};
    SkPoint delta = current_pos - last_mouse_pos_;
    pan_ += SkPoint::Make(delta.x() / zoom_, -delta.y() / zoom_);
    last_mouse_pos_ = current_pos;

    dirty_ = true;
  } else {
    // Convert screen coordinates to world coordinates
    current_mouse_pos_ = ScreenToWorld(
        SkPoint::Make(event.motion.x, (cached_height_ / 2.0) - event.motion.y));

    // Check for hover
    int old_hover_index = hover_segment_index_;
    hover_segment_index_ = -1;

    // Only check segments on enabled layers
    for (size_t i = 0; i < segment_paths_.size(); i++) {
      const auto &segment = segment_paths_[i];
      // Skip if layer is disabled
      if (!selected_layers_cache_[segment.layer_id]) {
        continue;
      }

      if (IsPointNearPath(segment.path, current_mouse_pos_,
                          segment.width * 1.5F)) {
        hover_segment_index_ = i;
        break;
      }
    }

    // Redraw if hover state changed
    if (old_hover_index != hover_segment_index_) {
      ComposeLayersToDrawable();
      dirty_ = true;
    }
  }
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
  uint8_t layer_id = LayerToId(inst->subtype, inst->layer);

  SkPoint starting = (*StartingPoint(k)) * (1.0 / factor_);

  while (IsLineSegment(k)) {
    SkPath segment_path;
    segment_path.moveTo(starting);
    SkPoint next;
    float segment_width;

    if (fs_->is_type(k, 0x01)) {
      const T01ArcSegment<kAMax> segment_inst = fs_->get_x01(k);
      segment_width = segment_inst.width / factor_;

      auto [cx, cy] = x01_center(&segment_inst);
      float scaled_cx = cx / factor_;
      float scaled_cy = cy / factor_;
      float scaled_r = (static_cast<double>(segment_inst.r)) / factor_;

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

      SkRect oval_bounds =
          SkRect::MakeXYWH(scaled_cx - scaled_r, scaled_cy - scaled_r,
                           scaled_r * 2, scaled_r * 2);

      segment_path.arcTo(oval_bounds, start_angle, sweep_angle, false);
      next = end;
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x15)) {
      const T15LineSegment<kAMax> segment_inst = fs_->get_x15(k);
      segment_width = segment_inst.width / factor_;
      next = SkPoint::Make(segment_inst.coords[2] / factor_,
                           segment_inst.coords[3] / factor_);
      segment_path.lineTo(next);
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x16)) {
      const T16LineSegment<kAMax> segment_inst = fs_->get_x16(k);
      segment_width = segment_inst.width / factor_;
      next = SkPoint::Make(segment_inst.coords[2] / factor_,
                           segment_inst.coords[3] / factor_);
      segment_path.lineTo(next);
      k = segment_inst.next;
    } else if (fs_->is_type(k, 0x17)) {
      const T17LineSegment<kAMax> segment_inst = fs_->get_x17(k);
      segment_width = segment_inst.width / factor_;
      next = SkPoint::Make(segment_inst.coords[2] / factor_,
                           segment_inst.coords[3] / factor_);
      segment_path.lineTo(next);
      k = segment_inst.next;
    } else {
      return;
    }

    // Store segment info
    segment_paths_.push_back({segment_path, segment_width, layer_id});

    // Add to layer paths as before
    size_t width_index = GetWidthIndex(segment_width);
    if (width_index < common_width_count_) {
      layer_paths_[layer_id].common_width_paths[width_index].addPath(
          segment_path);
    } else {
      auto &other_paths = layer_paths_[layer_id].other_width_paths;
      auto it =
          std::find_if(other_paths.begin(), other_paths.end(),
                       [segment_width](const auto &pair) {
                         return std::abs(pair.first - segment_width) < 0.001f;
                       });
      if (it != other_paths.end()) {
        it->second.addPath(segment_path);
      } else {
        other_paths.emplace_back(segment_width, segment_path);
      }
    }

    starting = next;
  }
}

void BrdWidget::DrawX28(const T28Shape<kAMax> *inst) {
  uint32_t k = inst->first_segment_ptr;
  uint8_t layer_id = LayerToId(inst->subtype, inst->layer);

  float current_width = -1;
  size_t width_index = common_width_count_;

  SkPath current_path;
  SkPoint starting = (*StartingPoint(k)) * (1.0 / factor_);
  SkPoint first_point = starting;
  current_path.moveTo(starting);

  while (IsLineSegment(k)) {
    SkPoint next;
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
          layer_paths_[layer_id].common_width_paths[width_index].addPath(
              current_path);
        } else {
          // Try to find an existing path with this width
          auto &other_paths = layer_paths_[layer_id].other_width_paths;
          auto it = std::find_if(other_paths.begin(), other_paths.end(),
                                 [current_width](const auto &pair) {
                                   return std::abs(pair.first - current_width) <
                                          0.001f;
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
  }

  // Ensure the path is properly closed by explicitly returning to start
  if (starting != first_point) {
    current_path.lineTo(first_point);
  }
  current_path.close();

  // Force path direction to be consistent
  current_path.setFillType(SkPathFillType::kWinding);

  layer_paths_[layer_id].filled_path.addPath(current_path);
}

auto BrdWidget::StartingPoint(uint32_t k) -> std::optional<SkPoint> {
  if (fs_->is_type(k, 0x01)) {
    const T01ArcSegment<kAMax> segment_inst = fs_->get_x01(k);
    return SkPoint::Make(static_cast<float>(segment_inst.coords[0]),
                         static_cast<float>(segment_inst.coords[1]));
  }
  if (fs_->is_type(k, 0x15)) {
    const T15LineSegment<kAMax> segment_inst = fs_->get_x15(k);
    return SkPoint::Make(static_cast<float>(segment_inst.coords[0]),
                         static_cast<float>(segment_inst.coords[1]));
  }
  if (fs_->is_type(k, 0x16)) {
    const T16LineSegment<kAMax> segment_inst = fs_->get_x16(k);
    return SkPoint::Make(static_cast<float>(segment_inst.coords[0]),
                         static_cast<float>(segment_inst.coords[1]));
  }
  if (fs_->is_type(k, 0x17)) {
    const T17LineSegment<kAMax> segment_inst = fs_->get_x17(k);
    return SkPoint::Make(static_cast<float>(segment_inst.coords[0]),
                         static_cast<float>(segment_inst.coords[1]));
  }
  return {};
}

auto BrdWidget::IsLineSegment(uint32_t k) -> bool {
  bool r = (fs_->is_type(k, 0x01)) || (fs_->is_type(k, 0x15)) ||
           (fs_->is_type(k, 0x16)) || (fs_->is_type(k, 0x17));
  // std::printf("isLineSegment k = 0x%08X, r = %d\n", ntohl(k), r);
  return r;
}

auto BrdWidget::GetWidthIndex(float width) -> size_t {
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
}

auto BrdWidget::ScreenToWorld(const SkPoint &screen_pos) -> SkPoint {
  return SkPoint::Make((screen_pos.x() / zoom_) - pan_.x(),
                       (screen_pos.y() / zoom_) - pan_.y());
}

auto BrdWidget::LayerToId(const uint8_t subtype, const uint8_t layer)
    -> uint8_t {
  if (subtype == 6 && layer <= 7) {
    return layer;
  }
  return 8;
}

// Add method to check if point is near path
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
