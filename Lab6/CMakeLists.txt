cmake_minimum_required(VERSION 3.15)
project(Lab6)

set(CMAKE_CXX_STANDARD 17)

add_executable(libusb libusb.cpp)

target_link_libraries(libusb  ${CMAKE_SOURCE_DIR}/libusb-1.0.a)