find_package(Qt6 COMPONENTS Widgets Gui Test REQUIRED)

set(CMAKE_AUTOMOC ON)

add_library(BrdGui mainwindow.cpp layer_item.cpp layer_model.cpp brdview.cpp)
target_link_libraries(BrdGui Parser Qt6::Widgets Qt6::Gui)

add_executable(brd_gui main.cpp brdview.cpp mainwindow.cpp layer_item.cpp layer_model.cpp)
target_link_libraries(brd_gui BrdGui)

add_subdirectory(test)
