add_library(Parser parser.cpp)
target_link_libraries(Parser Printing Structure)
target_link_libraries(Parser Boost::interprocess)

add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test GTest::gtest_main)
target_link_libraries(parser_test Parser)
gtest_discover_tests(parser_test)
