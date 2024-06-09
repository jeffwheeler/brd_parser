find_package(Qt6 COMPONENTS Widgets Gui Test REQUIRED)

qt_add_executable(gui_test gui_test.cpp)
add_test(NAME TestGui COMMAND gui_test)

target_link_libraries(gui_test PRIVATE BrdGui Qt::Widgets Qt::Test)
