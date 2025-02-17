#pragma once

// #include <GLES3/gl3.h>
// #include <SDL.h>

// #include "include/core/SkSurface.h"
// #include "webapp/brd_renderer.h"
// #include "webapp/layer_widget.h"

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Time.h>
#include <Magnum/Platform/EmscriptenApplication.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include "webapp/file_picker.h"
#include "webapp/brd_renderer.h"
#include "webapp/layer_widget.h"

namespace Cr = Corrade;
namespace Mn = Magnum;

using namespace Mn::Math::Literals;

class BrdViewerApp : public Mn::Platform::Application {
 public:
  explicit BrdViewerApp(const Arguments& arguments);

  void HandleFileUpload(const std::string& filepath);

  void drawEvent() override;

  void viewportEvent(ViewportEvent& event) override;

  void keyPressEvent(KeyEvent& event) override;
  void keyReleaseEvent(KeyEvent& event) override;

  void pointerPressEvent(PointerEvent& event) override;
  void pointerReleaseEvent(PointerEvent& event) override;
  void pointerMoveEvent(PointerMoveEvent& event) override;
  void scrollEvent(ScrollEvent& event) override;
  void textInputEvent(TextInputEvent& event) override;

 private:
  Mn::ImGuiIntegration::Context _imgui{Cr::NoCreate};

  LayerWidget layer_widget_;
  FilePickerWidget file_picker_widget_;
  BrdWidget brd_widget_;

  Mn::Color4 _clearColor = 0x72909aff_rgbaf;
};
