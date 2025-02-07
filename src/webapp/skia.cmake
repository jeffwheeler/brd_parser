include(ExternalProject)
string(JOIN "\n" SKIA_BUILD_ARGS
  is_debug=false
  is_official_build=false
  skia_emsdk_dir="$ENV{EMSDK}"
  skia_enable_fontmgr_custom_empty=true
  skia_enable_ganesh=true
  skia_enable_optimize_size=false
  skia_enable_skottie=false
  skia_enable_tools=false
  skia_gl_standard="webgl"
  skia_use_angle=false
  skia_use_libwebp_decode=false
  skia_use_lua=false
  skia_use_no_webp_encode=true
  skia_use_webgl=true
  skia_use_wuffs=false
  skia_use_icu=false
  target_cpu="wasm"
  target_os="emscripten"
)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/skia_args.gn ${SKIA_BUILD_ARGS})

ExternalProject_Add(
  # Not named `skia` to avoid conflict with the library name.
  skia_proj
  GIT_REPOSITORY https://skia.googlesource.com/skia
  GIT_TAG chrome/m134
  GIT_PROGRESS YES
  GIT_SHALLOW YES
  LOG_BUILD YES
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env
    GIT_SYNC_DEPS_SKIP_EMSDK=1
    python3 <SOURCE_DIR>/tools/git-sync-deps
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/skia_args.gn
    <BINARY_DIR>/args.gn
  BUILD_COMMAND <SOURCE_DIR>/bin/gn gen <BINARY_DIR> --root=<SOURCE_DIR>
  # Only build the ":skia" target. Run `ninja -t targets all` in the build
  # directory to see all available targets.
  COMMAND ninja -j20 -C <BINARY_DIR> ":skia"
  BUILD_BYPRODUCTS <BINARY_DIR>/libskia.a
  INSTALL_COMMAND ""
  EXCLUDE_FROM_ALL YES
)
ExternalProject_Get_Property(skia_proj SOURCE_DIR BINARY_DIR)
set(skia_SOURCE_DIR ${SOURCE_DIR})
set(skia_BINARY_DIR ${BINARY_DIR})

add_library(skia STATIC IMPORTED)
set_target_properties(skia PROPERTIES
  IMPORTED_LOCATION ${skia_BINARY_DIR}/libskia.a
  INTERFACE_INCLUDE_DIRECTORIES ${skia_SOURCE_DIR}
)
