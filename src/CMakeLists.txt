# cmake -DBUILD_GUI=OFF -DCMAKE_TOOLCHAIN_FILE=/Users/jeff/Documents/Source/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=/Users/jeff/Documents/Source/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten -DCMAKE_INSTALL_PREFIX=/Users/jeff/Documents/Source/brd_parser/webapp_out/
# cmake --build ../build_clang --target webapp && cmake --install ../build_clang/webapp/

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0167 NEW)

# set(CMAKE_SYSTEM_PROCESSOR "wasm")

option(BUILD_GUI "Build Qt GUI")
option(BUILD_WEBAPP "Build WASM web app")

if (BUILD_WEBAPP)
  # This will add the vcpkg dependencies required for building the webapp.
  list(APPEND VCPKG_MANIFEST_FEATURES "webapp")
endif ()

project(AllegroParser)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("-g")
# add_compile_options("-O0")
add_compile_options("-O3")
add_compile_options("-Wall")

# https://groups.google.com/g/skia-discuss/c/UZil9nCWGhg
# add_compile_options("-DSK_TRIVIAL_ABI=[[clang::trivial_abi]]")
# add_compile_options("-DMAGNUM_TARGET_WEBGL=1")
# add_compile_options("-DCK_ENABLE_WEBGL=1")
# add_compile_options("-DSK_GL=1")
# add_compile_options("-DSK_DISABLE_TRACING=1")
# add_compile_options("-DSKNX_NO_SIMD=1")
# add_compile_options("-DSK_FORCE_8_BYTE_ALIGNMENT=1")

if (MSVC)
  # Fix Boost compatibility issue with MSVC:
  # https://stackoverflow.com/a/78034542/647720
  add_compile_options("/EHsc")

  # GoogleTest fails with /W3.
  add_compile_options("/W2")
else ()
  add_compile_options("-Werror")
  add_compile_options("-Wextra")
endif (MSVC)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Enable color output from compilers even when using Ninja, which otherwise
# suppresses the colors. See https://stackoverflow.com/a/73349744/647720.
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(MAGNUM_WITH_IMGUI ON)
set(MAGNUM_WITH_EMSCRIPTENAPPLICATION ON)
set(MAGNUM_WITH_MESHTOOLS ON)

set(MAGNUM_TARGET_WEBGL ON)
set(MAGNUM_TARGET_GLES ON)
set(MAGNUM_TARGET_GLES2 OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP YES
)
if (BUILD_WEBAPP)
  FetchContent_Declare(
    em_file_browser
    GIT_REPOSITORY https://github.com/Armchair-Software/emscripten-browser-file.git
    GIT_TAG b7312b42dc92dc791dac53a7df96e231a8751607
  )
  FetchContent_Declare(
      corrade
      GIT_REPOSITORY https://github.com/mosra/corrade.git
      GIT_TAG 9ade86c04bd4d930b0c897ec82d2833eea8ceaa4
  )
  FetchContent_Declare(
      magnum
      GIT_REPOSITORY https://github.com/mosra/magnum.git
      GIT_TAG 15f76e08b68f89f1e971ab57fa9cd6747018fd16
  )
  FetchContent_Declare(
      magnum_integration
      GIT_REPOSITORY https://github.com/mosra/magnum-integration.git
      GIT_TAG ed2730d95836fdcbdbb659c5c070bcde2eb07175
  )
  FetchContent_MakeAvailable(googletest em_file_browser corrade magnum magnum_integration)
else ()
  FetchContent_MakeAvailable(googletest)
endif ()

enable_testing()

# find_package(imgui CONFIG REQUIRED)
# find_package(Boost REQUIRED)
find_package(boost_format REQUIRED CONFIG)
find_package(boost_interprocess REQUIRED CONFIG)
find_package(boost_unordered REQUIRED CONFIG)
include_directories(${Boost_INCLUDE_DIRS})

# set(WITH_IMGUI ON CACHE BOOL "" FORCE)

# This allows headers to be imported from "lib/.../*.h"
include_directories(.)

add_subdirectory(lib)
add_subdirectory(brd_cli)
if (BUILD_GUI)
    add_subdirectory(brd_gui)
endif ()
if (BUILD_WEBAPP)
    add_subdirectory(webapp)
endif ()
add_subdirectory(test)
