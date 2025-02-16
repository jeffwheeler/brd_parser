# include(skia.cmake)

find_package(imgui CONFIG REQUIRED)

set(MAGNUM_WITH_IMGUI ON)
set(MAGNUM_WITH_EMSCRIPTENAPPLICATION ON)
set(MAGNUM_WITH_MESHTOOLS ON)

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
  # main.cpp
  webapp.cpp
  layer_widget.cpp
  brd_renderer.cpp
  file_picker.cpp
  app_state.cpp
)
target_link_libraries(webapp PRIVATE
  Magnum::EmscriptenApplication
  Magnum::MeshTools
  Magnum::Primitives
  MagnumIntegration::ImGui
)
# target_include_directories(webapp PRIVATE ${skia_SOURCE_DIR})
# target_link_libraries(webapp PRIVATE skia)
target_link_libraries(webapp PRIVATE imgui::imgui)
target_link_libraries(webapp PRIVATE Parser)

target_include_directories(webapp PRIVATE ${em_file_browser_SOURCE_DIR})

target_compile_options(webapp PRIVATE "--use-port=sdl2")

target_link_options(webapp PRIVATE "-sALLOW_MEMORY_GROWTH")
# Enabling DWARF support prevents binaryen optimizations
target_link_options(webapp PRIVATE "-g")
target_link_options(webapp PRIVATE "-sFULL_ES2=1")
target_link_options(webapp PRIVATE "-sFULL_ES3=1")
target_link_options(webapp PRIVATE "-sUSE_WEBGL2=1")
target_link_options(webapp PRIVATE "-sMAX_WEBGL_VERSION=2")
target_link_options(webapp PRIVATE "-sEXPORTED_FUNCTIONS=[\"_main\", \"_malloc\", \"_free\"]")
target_link_options(webapp PRIVATE "-sEXPORTED_RUNTIME_METHODS=[\"ccall\"]")

install(TARGETS webapp DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES
  webapp.html webapp.css setup.js
  ${MAGNUM_EMSCRIPTENAPPLICATION_JS}
  ${MAGNUM_WEBAPPLICATION_CSS}
  DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES
  ${CMAKE_BINARY_DIR}/bin/webapp.wasm
  DESTINATION ${CMAKE_INSTALL_PREFIX})
# install(FILES
#   # ${CMAKE_BINARY_DIR}/webapp/webapp.data
#   ${CMAKE_BINARY_DIR}/webapp/webapp.wasm
#   DESTINATION ${CMAKE_INSTALL_PREFIX})
