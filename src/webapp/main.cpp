#include <emscripten.h>

#include "webapp/webapp.h"

void main_loop() { BrdViewerApp::Render(); }

auto main() -> int {
  // Create renderer
  new BrdViewerApp();

  // Set main loop
  emscripten_set_main_loop(main_loop, 0, true);

  return 0;
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
void handleDroppedFile(const char* filepath) {
  BrdViewerApp::HandleFileUpload(filepath);
}
}
