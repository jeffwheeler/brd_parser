#include <emscripten.h>

#include "webapp/webapp.h"

// Global renderer
BrdViewerApp* g_renderer = nullptr;

void main_loop() { g_renderer->Render(); }

auto main() -> int {
  // Create renderer
  g_renderer = new BrdViewerApp();

  // Set main loop
  emscripten_set_main_loop(main_loop, 0, true);

  return 0;
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
void handleDroppedFile(const char* filepath) {
  if (g_renderer != nullptr) {
    g_renderer->handleNewFile(filepath);
  }
}
}
