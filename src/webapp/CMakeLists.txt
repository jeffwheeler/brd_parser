# include(skia.cmake)

find_package(imgui CONFIG REQUIRED)

set(CMAKE_MODULE_PATH "${corrade_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${magnum_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${magnum_integration_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

find_package(Corrade REQUIRED)
find_package(Magnum REQUIRED
  EmscriptenApplication
  Primitives
)
find_package(MagnumIntegration REQUIRED ImGui)

add_executable(webapp
  webapp.cpp
  line_shader.cpp
  layer_widget.cpp
  brd_renderer.cpp
  file_picker.cpp
  app_state.cpp
)

target_link_libraries(webapp PRIVATE
  imgui::imgui
  Magnum::EmscriptenApplication
  Magnum::MeshTools
  Magnum::Primitives
  MagnumIntegration::ImGui
  Parser
)

target_include_directories(webapp PRIVATE ${em_file_browser_SOURCE_DIR})

target_compile_options(webapp PRIVATE "--use-port=sdl2")

target_link_options(webapp PRIVATE "-sALLOW_MEMORY_GROWTH")
# Enabling DWARF support prevents binaryen optimizations
# target_link_options(webapp PRIVATE "-g")
target_link_options(webapp PRIVATE "-sFULL_ES2=1")
target_link_options(webapp PRIVATE "-sFULL_ES3=1")
target_link_options(webapp PRIVATE "-sUSE_WEBGL2=1")
target_link_options(webapp PRIVATE "-sMAX_WEBGL_VERSION=2")
target_link_options(webapp PRIVATE "-sEXPORTED_FUNCTIONS=[\"_main\", \"_malloc\", \"_free\"]")
target_link_options(webapp PRIVATE "-sEXPORTED_RUNTIME_METHODS=[\"ccall\"]")

install(TARGETS webapp DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES
  webapp.html
  webapp.css
  setup.js
  "$<TARGET_FILE_DIR:webapp>/webapp.js"
  "$<TARGET_FILE_DIR:webapp>/webapp.wasm"
  ${MAGNUM_EMSCRIPTENAPPLICATION_JS}
  ${MAGNUM_WEBAPPLICATION_CSS}
  DESTINATION ${CMAKE_INSTALL_PREFIX})
