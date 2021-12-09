cmake_minimum_required(VERSION 3.17)
project(Lab9)

set(CMAKE_CXX_STANDARD 17)
MATH(EXPR stack_size "512 * 1024 * 1024") # 512 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}") # Increase stack size

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")

add_executable(Lab9 cacheAss.cpp)