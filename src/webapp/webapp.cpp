#include "webapp.h"

#include <emscripten/html5.h>

#include "lib/parser/parser.h"
#include "webapp/app_state.h"

BrdViewerApp::BrdViewerApp(const Arguments& arguments)
    : Magnum::Platform::
          Application{arguments, Configuration{}
                                     .setTitle("Magnum ImGui Example")
                                     .setWindowFlags(
                                         Configuration::WindowFlag::Resizable)},
      file_picker_widget_(this) {
  _imgui = Magnum::ImGuiIntegration::Context(
      Magnum::Vector2{windowSize()} / dpiScaling(), windowSize(),
      framebufferSize());

  /* Set up proper blending to be used by ImGui. There's a great chance
     you'll need this exact behavior for the rest of your scene. If not, set
     this only for the drawFrame() call. */
  Magnum::GL::Renderer::setBlendEquation(
      Magnum::GL::Renderer::BlendEquation::Add,
      Magnum::GL::Renderer::BlendEquation::Add);
  Magnum::GL::Renderer::setBlendFunction(
      Magnum::GL::Renderer::BlendFunction::SourceAlpha,
      Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);

#if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
  /* Have some sane speed, please */
  setMinimalLoopPeriod(16.0_msec);
#endif
}

void BrdViewerApp::drawEvent() {
  Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

  _imgui.newFrame();

  /* Enable text input, if needed */
  if (ImGui::GetIO().WantTextInput && !isTextInputActive()) {
    startTextInput();
  } else if (!ImGui::GetIO().WantTextInput && isTextInputActive()) {
    stopTextInput();
  }

  brd_widget_.Draw();

  layer_widget_.Draw();
  file_picker_widget_.Draw();

  /* Update application cursor */
  _imgui.updateApplicationCursor(*this);

  _imgui.drawFrame();

  swapBuffers();
  redraw();
}

void BrdViewerApp::viewportEvent(ViewportEvent& event) {
  Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

  _imgui.relayout(Magnum::Vector2{event.windowSize()} / event.dpiScaling(),
                  event.windowSize(), event.framebufferSize());

  brd_widget_.UpdateScreenRatio();
}

void BrdViewerApp::keyPressEvent(KeyEvent& event) {
  if (_imgui.handleKeyPressEvent(event)) {
    return;
  }
}

void BrdViewerApp::keyReleaseEvent(KeyEvent& event) {
  if (_imgui.handleKeyReleaseEvent(event)) {
    return;
  }
}

void BrdViewerApp::pointerPressEvent(PointerEvent& event) {
  if (_imgui.handlePointerPressEvent(event)) {
    return;
  }

  brd_widget_.HandleMouseDown(event);
}

void BrdViewerApp::pointerReleaseEvent(PointerEvent& event) {
  if (_imgui.handlePointerReleaseEvent(event)) {
    return;
  }

  brd_widget_.HandleMouseUp(event);
}

void BrdViewerApp::pointerMoveEvent(PointerMoveEvent& event) {
  if (_imgui.handlePointerMoveEvent(event)) {
    return;
  }

  brd_widget_.HandleMouseMove(event);
}

void BrdViewerApp::scrollEvent(ScrollEvent& event) {
  if (_imgui.handleScrollEvent(event)) {
    /* Prevent scrolling the page */
    event.setAccepted();
    return;
  }

  brd_widget_.HandleMouseWheel(event);
}

void BrdViewerApp::textInputEvent(TextInputEvent& event) {
  if (_imgui.handleTextInputEvent(event)) {
    return;
  }
}

void BrdViewerApp::HandleFileUpload(const std::string& filepath) {
  auto fs = parse_file(filepath);
  if (fs) {
    AppState::CurrentFile() = std::make_shared<File<kAMax>>(std::move(*fs));
    brd_widget_.UpdateFile();
    layer_widget_.UpdateFile();
  } else {
    emscripten_log(EM_LOG_ERROR, "Failed to parse dropped file");
  }
}

MAGNUM_EMSCRIPTENAPPLICATION_MAIN(BrdViewerApp)
