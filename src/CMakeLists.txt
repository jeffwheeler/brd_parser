cmake_minimum_required(VERSION 3.12)

project(AllegroParser)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-O3") # Use -Wall here
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/06f44bc951046150f1348598854b211afdcf37fc.zip
  DOWNLOAD_EXTRACT_TIMESTAMP YES
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# This allows headers to be imported from "lib/.../*.h"
include_directories(.)

add_subdirectory(lib)
add_subdirectory(app EXCLUDE_FROM_ALL)
add_subdirectory(test)
