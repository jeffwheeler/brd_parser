#include "webapp.h"

#include <emscripten/html5.h>

ImGuiExample::ImGuiExample(const Arguments& arguments)
    : Magnum::Platform::Application{
          arguments,
          Configuration{}
              .setTitle("Magnum ImGui Example")
              .setWindowFlags(Configuration::WindowFlag::Resizable)} {
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

void ImGuiExample::drawEvent() {
  Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

  _imgui.newFrame();

  /* Enable text input, if needed */
  if (ImGui::GetIO().WantTextInput && !isTextInputActive()) {
    startTextInput();
  } else if (!ImGui::GetIO().WantTextInput && isTextInputActive()) {
    stopTextInput();
  }

  /* 1. Show a simple window.
     Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appear in
     a window called "Debug" automatically */
  {
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("Float", &_floatValue, 0.0F, 1.0F);
    if (ImGui::ColorEdit3("Clear Color", _clearColor.data())) {
      Magnum::GL::Renderer::setClearColor(_clearColor);
    }
    if (ImGui::Button("Test Window")) {
      _showDemoWindow ^= 1;
    }
    if (ImGui::Button("Another Window")) {
      _showAnotherWindow ^= 1;
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / static_cast<Magnum::Double>(ImGui::GetIO().Framerate),
                static_cast<Magnum::Double>(ImGui::GetIO().Framerate));
  }

  /* 2. Show another simple window, now using an explicit Begin/End pair */
  if (_showAnotherWindow) {
    ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("Another Window", &_showAnotherWindow);
    ImGui::Text("Hello");
    ImGui::End();
  }

  /* 3. Show the ImGui demo window. Most of the sample code is in
     ImGui::ShowDemoWindow() */
  if (_showDemoWindow) {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ImGui::ShowDemoWindow();
  }

  /* Update application cursor */
  _imgui.updateApplicationCursor(*this);

  /* Set appropriate states. If you only draw ImGui, it is sufficient to
     just enable blending and scissor test in the constructor. */
  Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
  Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::ScissorTest);
  Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::FaceCulling);
  Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::DepthTest);

  _imgui.drawFrame();

  /* Reset state. Only needed if you want to draw something else with
     different state after. */
  Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
  Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
  Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::ScissorTest);
  Magnum::GL::Renderer::disable(Magnum::GL::Renderer::Feature::Blending);

  swapBuffers();
  redraw();
}

void ImGuiExample::viewportEvent(ViewportEvent& event) {
  Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

  _imgui.relayout(Magnum::Vector2{event.windowSize()} / event.dpiScaling(),
                  event.windowSize(), event.framebufferSize());
}

void ImGuiExample::keyPressEvent(KeyEvent& event) {
  if (_imgui.handleKeyPressEvent(event)) {
    return;
  }
}

void ImGuiExample::keyReleaseEvent(KeyEvent& event) {
  if (_imgui.handleKeyReleaseEvent(event)) {
    return;
  }
}

void ImGuiExample::pointerPressEvent(PointerEvent& event) {
  if (_imgui.handlePointerPressEvent(event)) {
    return;
  }
}

void ImGuiExample::pointerReleaseEvent(PointerEvent& event) {
  if (_imgui.handlePointerReleaseEvent(event)) {
    return;
  }
}

void ImGuiExample::pointerMoveEvent(PointerMoveEvent& event) {
  if (_imgui.handlePointerMoveEvent(event)) {
    return;
  }
}

void ImGuiExample::scrollEvent(ScrollEvent& event) {
  if (_imgui.handleScrollEvent(event)) {
    /* Prevent scrolling the page */
    event.setAccepted();
    return;
  }
}

void ImGuiExample::textInputEvent(TextInputEvent& event) {
  if (_imgui.handleTextInputEvent(event)) {
    return;
  }
}
