#include "webapp.h"

#include <emscripten/html5.h>

#include <memory>

#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPaint.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"

// ImGui headers
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "lib/parser/parser.h"

BrdViewerApp::BrdViewerApp() {
  app_singleton = this;

  SkGraphics::Init();

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    emscripten_log(EM_LOG_ERROR, "SDL initialization failed: %s",
                   SDL_GetError());
    exit(-1);
  }

  // Setup SDL and create window
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // Get window dimensions from browser
  width_ = EM_ASM_INT({ return window.innerWidth; });
  height_ = EM_ASM_INT({ return window.innerHeight; });

  window_ = SDL_CreateWindow("PCB Viewer", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, width_, height_,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window_ == nullptr) {
    emscripten_log(EM_LOG_ERROR, "Window creation failed: %s", SDL_GetError());
    exit(-1);
  }

  // Create OpenGL context
  SDL_GLContext gl_context = SDL_GL_CreateContext(window_);
  SDL_GL_MakeCurrent(window_, gl_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync

  // Initialize WebGL
  EmscriptenWebGLContextAttributes attrs;
  emscripten_webgl_init_context_attributes(&attrs);
  attrs.preserveDrawingBuffer = true;

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx =
      emscripten_webgl_create_context("#canvas", &attrs);
  emscripten_webgl_make_context_current(ctx);

  // Initialize ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window_, gl_context);
  ImGui_ImplOpenGL3_Init("#version 300 es");

  // Set up initial canvas size to window size
  emscripten_set_canvas_element_size("#canvas", width_, height_);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  CreateSkiaSurface();
};

void BrdViewerApp::Render() {
  if (app_singleton != nullptr) {
    app_singleton->RenderImpl();
  }
}

void BrdViewerApp::CreateSkiaSurface() {
  // Get device pixel ratio
  device_pixel_ratio_ = EM_ASM_DOUBLE({ return window.devicePixelRatio || 1; });

  // Calculate actual pixel dimensions
  int pixelWidth = static_cast<int>(width_ * device_pixel_ratio_);
  int pixelHeight = static_cast<int>(height_ * device_pixel_ratio_);

  // Initialize Skia's GPU context
  auto interface = GrGLMakeNativeInterface();
  gr_context_ = GrDirectContexts::MakeGL(interface).release();

  // Create Skia surface
  GrGLFramebufferInfo framebufferInfo;
  framebufferInfo.fFBOID = 0;
  framebufferInfo.fFormat = GL_RGBA8;

  auto backendRenderTarget = GrBackendRenderTargets::MakeGL(
      pixelWidth, pixelHeight, 4, 8, framebufferInfo);

  skia_surface_ =
      SkSurfaces::WrapBackendRenderTarget(
          gr_context_, backendRenderTarget, kTopLeft_GrSurfaceOrigin,
          kRGBA_8888_SkColorType, nullptr, nullptr)
          .release();

  emscripten_log(EM_LOG_INFO, "Skia surface created: %d x %d (Pixel Ratio: %f)",
                 pixelWidth, pixelHeight, device_pixel_ratio_);
}

void BrdViewerApp::Resize(int width, int height) {
  emscripten_log(EM_LOG_INFO, "Resize, magic: 0x%08X", fs_->hdr->magic);
  width_ = width;
  height_ = height;
  CreateSkiaSurface();
  emscripten_set_canvas_element_size("#canvas", width_, height_);
  brd_widget_.MarkDirty();
}

void BrdViewerApp::RenderImpl() {
  // Poll and handle events
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
      brd_widget_.MarkDirty();
      continue;
    }

    switch (event.type) {
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          int updated_width = event.window.data1;
          int updated_height = event.window.data2;
          Resize(updated_width, updated_height);
        }
        break;
      case SDL_MOUSEWHEEL:
        brd_widget_.HandleMouseWheel(event);
        break;
      case SDL_MOUSEBUTTONDOWN:
        brd_widget_.HandleMouseDown(event);
        break;
      case SDL_MOUSEBUTTONUP:
        brd_widget_.HandleMouseUp(event);
        break;
      case SDL_MOUSEMOTION:
        brd_widget_.HandleMouseMove(event);
        break;
      default:
        // Unrecognized event type
        break;
    }
  }

  // Start ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  RenderSkia();
  RenderImGuiOverlay();

  // Render ImGui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap buffers
  SDL_GL_SwapWindow(window_);
}

void BrdViewerApp::RenderSkia() {
  if (skia_surface_ == nullptr) {
    return;
  }

  brd_widget_.Draw(skia_surface_, device_pixel_ratio_,
                   layer_widget_.selected_layers_);

  // Flush the Skia canvas
  gr_context_->flushAndSubmit();
}

void BrdViewerApp::RenderImGuiOverlay() {
  // ImGui rendering
  ImGui::SetNextWindowPos(ImVec2(width_ * 0.65F, height_ * 0.1F),
                          ImGuiCond_FirstUseEver);
  {
    layer_widget_.Draw();
  }

  ImGui::SetNextWindowPos(ImVec2(width_ * 0.15F, height_ * 0.1F),
                          ImGuiCond_FirstUseEver);
  {
    file_picker_widget_.Draw();
  }
}

void BrdViewerApp::HandleFileUploadImpl(const std::string& filepath) {
  auto fs = parse_file(filepath.c_str());
  if (fs) {
    emscripten_log(EM_LOG_INFO, "Parsed dropped file successfully");
    fs_ = std::make_shared<File<kAMax>>(std::move(*fs));
    layer_widget_.UpdateFile(fs_);
    brd_widget_.UpdateFile(fs_);
  } else {
    emscripten_log(EM_LOG_ERROR, "Failed to parse dropped file");
  }
}

void BrdViewerApp::HandleFileUpload(const std::string& filepath) {
  if (app_singleton != nullptr) {
    app_singleton->HandleFileUploadImpl(filepath);
  }
}
