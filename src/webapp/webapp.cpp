#include "webapp.h"

#include <emscripten/html5.h>

#include "lib/parser/parser.h"
#include "webapp/app_state.h"

BrdViewerApp::BrdViewerApp(const Arguments& arguments)
    : Mn::Platform::Application{arguments,
                                Configuration{}
                                    .setTitle("Mn ImGui Example")
                                    .setWindowFlags(
                                        Configuration::WindowFlag::Resizable)},
      file_picker_widget_(this) {
  _imgui =
      Mn::ImGuiIntegration::Context(Mn::Vector2{windowSize()} / dpiScaling(),
                                    windowSize(), framebufferSize());

  /* Set up proper blending to be used by ImGui. There's a great chance
     you'll need this exact behavior for the rest of your scene. If not, set
     this only for the drawFrame() call. */
  Mn::GL::Renderer::setBlendEquation(Mn::GL::Renderer::BlendEquation::Add,
                                     Mn::GL::Renderer::BlendEquation::Add);
  Mn::GL::Renderer::setBlendFunction(
      Mn::GL::Renderer::BlendFunction::SourceAlpha,
      Mn::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
  Mn::GL::Renderer::disable(Mn::GL::Renderer::Feature::FaceCulling);
  Mn::GL::Renderer::enable(Mn::GL::Renderer::Feature::Blending);

#if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
  /* Have some sane speed, please */
  setMinimalLoopPeriod(16.0_msec);
#endif
}

void BrdViewerApp::drawEvent() {
  Mn::GL::defaultFramebuffer.clear(Mn::GL::FramebufferClear::Color);

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

  Mn::GL::Renderer::enable(Mn::GL::Renderer::Feature::ScissorTest);
  Mn::GL::Renderer::disable(Mn::GL::Renderer::Feature::DepthTest);
  _imgui.drawFrame();
  Mn::GL::Renderer::enable(Mn::GL::Renderer::Feature::DepthTest);
  Mn::GL::Renderer::disable(Mn::GL::Renderer::Feature::ScissorTest);

  swapBuffers();
  redraw();
}

void BrdViewerApp::viewportEvent(ViewportEvent& event) {
  Mn::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

  _imgui.relayout(Mn::Vector2{event.windowSize()} / event.dpiScaling(),
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
  // Clear the existing file to ensure there is plenty of memory for the next
  // file.
  AppState::CurrentFile() = nullptr;

  auto fs = parse_file(filepath);
  if (fs) {
    AppState::CurrentFile() = std::move(fs);
    brd_widget_.UpdateFile();
    layer_widget_.UpdateFile();
  } else {
    emscripten_log(EM_LOG_ERROR, "Failed to parse dropped file");
  }
}

MAGNUM_EMSCRIPTENAPPLICATION_MAIN(BrdViewerApp)
