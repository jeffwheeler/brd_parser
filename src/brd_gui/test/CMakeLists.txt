find_package(Qt6 COMPONENTS Widgets Gui Test REQUIRED)

qt_add_executable(mainwindow_test mainwindow_test.cpp)
add_test(NAME TestMainWindow COMMAND mainwindow_test)

qt_add_executable(brdview_test brdview_test.cpp)
add_test(NAME TestBrdView COMMAND brdview_test)

target_link_libraries(mainwindow_test PRIVATE BrdGui Qt::Widgets Qt::Test)
target_link_libraries(brdview_test PRIVATE BrdGui Qt::Widgets Qt::Test)
