include(skia.cmake)

find_package(imgui CONFIG REQUIRED)

add_executable(webapp main.cpp webapp.cpp layer_widget.cpp brd_renderer.cpp)
add_dependencies(webapp skia_proj)
# target_include_directories(webapp PRIVATE ${skia_SOURCE_DIR})
target_link_libraries(webapp PRIVATE skia)
target_link_libraries(webapp PRIVATE imgui::imgui)
target_link_libraries(webapp PRIVATE Parser)

target_compile_options(webapp PRIVATE "--use-port=sdl2")

# This is probably very slow, just for testing...
# target_link_options(webapp PRIVATE
#   "--preload-file"
#   ${CMAKE_BINARY_DIR}/test/data/beagle_y/BeagleY-AI_PCB_V1.0_240105.brd@/example_file.brd)
target_link_options(webapp PRIVATE "-sALLOW_MEMORY_GROWTH")
# Enabling DWARF support prevents binaryen optimizations
# target_link_options(webapp PRIVATE "-g")
target_link_options(webapp PRIVATE "-sFULL_ES2=1")
target_link_options(webapp PRIVATE "-sFULL_ES3=1")
target_link_options(webapp PRIVATE "-sUSE_WEBGL2=1")
target_link_options(webapp PRIVATE "-sMIN_WEBGL_VERSION=2")
target_link_options(webapp PRIVATE "-sMAX_WEBGL_VERSION=2")
target_link_options(webapp PRIVATE "-sEXPORTED_RUNTIME_METHODS=[\"ccall\"]")

install(TARGETS webapp DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES
  webapp.html webapp.css setup.js
  DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES
  # ${CMAKE_BINARY_DIR}/webapp/webapp.data
  ${CMAKE_BINARY_DIR}/webapp/webapp.wasm
  DESTINATION ${CMAKE_INSTALL_PREFIX})
