
cmake_minimum_required(VERSION 3.9)
project(201703)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --static --pedantic")

add_executable(201703 main.cpp binary_tree.hpp console_ui.hpp test/test_binary_tree.cpp test/test_binary_tree.hpp tree_adapter.hpp tree_parse.hpp test/test_tree_parse.cpp test/test_tree_parse.hpp test/test_tree_adapter.cpp test/test_tree_adapter.hpp save_load.hpp)