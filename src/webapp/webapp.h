#pragma once

#include <GLES3/gl3.h>
#include <SDL.h>

#include "include/core/SkSurface.h"
#include "lib/structure/types.h"
#include "webapp/brd_renderer.h"
#include "webapp/file_picker.h"
#include "webapp/layer_widget.h"

class BrdViewerApp {
 public:
  explicit BrdViewerApp();

  static auto App() -> BrdViewerApp&;

  void Render();
  void HandleFileUpload(const std::string& filepath);

 private:
  SDL_Window* window_;
  SkSurface* skia_surface_ = nullptr;
  GrDirectContext* gr_context_ = nullptr;

  LayerWidget layer_widget_;
  BrdWidget brd_widget_;

  std::shared_ptr<File<kAMax>> fs_;

  double device_pixel_ratio_ = 1.0;
  int width_ = 0;
  int height_ = 0;

  void RenderSkia();
  void RenderImGuiOverlay();
  void Resize(int width, int height);
  void CreateSkiaSurface();
};
