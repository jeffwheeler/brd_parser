find_package(Qt6 COMPONENTS Widgets Gui REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(brd_gui main.cpp brdview.cpp mainwindow.cpp)
target_link_libraries(brd_gui Qt6::Widgets Qt6::Gui)
target_link_libraries(brd_gui Parser)
