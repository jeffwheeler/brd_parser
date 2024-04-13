cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(AllegroParser)

option(BUILD_GUI "Build Qt GUI")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3 -Wall")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

IF(MSVC)
    # https://stackoverflow.com/questions/50133783/c-cmake-build-error-undefined-reference-to-boostthrow-exceptionstdexcep
    add_compile_options(/EHsc)
ENDIF(MSVC)

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
