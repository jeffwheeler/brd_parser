add_library(Structure types.cpp cadence_fp.cpp utils.cpp)
target_link_libraries(Structure Boost::interprocess)
