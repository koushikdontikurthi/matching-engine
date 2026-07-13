cmake_minimum_required(VERSION 3.10)
project(MatchingEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(matching_engine src/OrderBook.cpp src/main.cpp)
target_include_directories(matching_engine PRIVATE src)
