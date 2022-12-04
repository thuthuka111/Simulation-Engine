cmake_minimum_required(VERSION 3.12)
project(FakeBasketball)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES
        main.cpp
        OpenGL.cpp
        GameObjects/*.cpp
        Collideable/*.cpp
)

add_subdirectory(glew)
add_subdirectory(glfw)

include_directories(glfw/include)
include_directories(glew/include)
include_directories(glm)

add_executable(FakeBasketball ${SOURCE_FILES})

target_link_libraries(FakeBasketball glfw libglew_static)