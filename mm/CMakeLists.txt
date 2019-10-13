cmake_minimum_required(VERSION 3.8)
project(cpp-concurrency)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++17")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

add_executable(code_reorder               01_code_reorder.cpp)