cmake_minimum_required(VERSION 3.15)
project(Lab5)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "D:/Libraries/OpenCV/opencv/build")
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(Lab5 opencv.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})