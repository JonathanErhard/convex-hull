cmake_minimum_required(VERSION 2.8)
project(convex-hull)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} .)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})