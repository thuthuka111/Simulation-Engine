cmake_minimum_required(VERSION 3.12)
project(FakeBasketball)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(glew)
add_subdirectory(glfw)

include_directories(glfw/include)
include_directories(glew/include)

add_executable(FakeBasketball main.cpp OpenGL.cpp OpenGL.h)

target_link_libraries(FakeBasketball glfw libglew_static)