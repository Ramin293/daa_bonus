cmake_minimum_required(VERSION 3.16)
project(bst_vs_hash_table LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(daa_bonus src/main.cpp)
target_include_directories(daa_bonus PRIVATE include)

add_executable(benchmark src/benchmark.cpp)
target_include_directories(benchmark PRIVATE include)
