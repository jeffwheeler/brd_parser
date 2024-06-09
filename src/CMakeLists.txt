cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(AllegroParser)

option(BUILD_GUI "Build Qt GUI")

set(CMAKE_CXX_STANDARD 17)

add_compile_options("-O3")
add_compile_options("-Wall")

IF(MSVC)
    # Fix Boost compatibility issue with MSVC:
    # https://stackoverflow.com/a/78034542/647720
    add_compile_options("/EHsc")

    # GoogleTest fails with /W3.
    add_compile_options("/W2")
ELSE()
    add_compile_options("-Werror")
ENDIF(MSVC)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Enable color output from compilers even when using Ninja, which otherwise
# suppresses the colors. See https://stackoverflow.com/a/73349744/647720.
set(CMAKE_COLOR_DIAGNOSTICS ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/06f44bc951046150f1348598854b211afdcf37fc.zip
    DOWNLOAD_EXTRACT_TIMESTAMP YES
)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_package(Boost 1.71 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# This allows headers to be imported from "lib/.../*.h"
include_directories(.)

add_subdirectory(lib)
add_subdirectory(brd_cli)
if (BUILD_GUI)
    add_subdirectory(brd_gui)
endif ()
add_subdirectory(test)
