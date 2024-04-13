add_subdirectory(data)

IF(MSVC)
    # https://stackoverflow.com/questions/6832666/lnk2019-when-including-asio-headers-solution-generated-with-cmake
    add_compile_options(/EHsc)
ENDIF(MSVC)

# Additional tests can be added to the `proprietary_designs` directory without
# publicly sharing. This will be ignored if the folder does not exist.
#
# This folder is also ignored in the project's `.gitignore`.
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/proprietary_designs)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/proprietary_designs)
endif ()
