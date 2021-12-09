cmake_minimum_required(VERSION 3.15)
project(Lab7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1") # for SSE

MATH(EXPR stack_size "148 * 1024 * 1024") # 148 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}") # Increase stack size

set(OPENBLAS_PATH "D:/Libraries/OpenBLAS 0.3.7")
include_directories("${OPENBLAS_PATH}/include")

add_executable(NoSSE NoSSE.cpp) # 134 Mb
add_executable(SSE SSE.cpp) # 134 Mb
add_executable(OpenBLAS OpenBLAS.cpp) # 112 Mb

target_link_libraries(OpenBLAS ${OPENBLAS_PATH}/lib/libopenblas.a)

