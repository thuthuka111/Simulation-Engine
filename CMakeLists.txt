cmake_minimum_required(VERSION 3.12)
project(FakeBasketball)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES
        main.cpp
        OpenGL.cpp
        RigidBodies/*.cpp
        Collideable/*.cpp
)

add_subdirectory(glew)
add_subdirectory(glfw)

include_directories(glfw/include)
include_directories(glew/include)
include_directories(glm)

add_executable(FakeBasketball ${SOURCE_FILES})

#include(C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(Freetype REQUIRED)
target_link_libraries(FakeBasketball PRIVATE Freetype::Freetype)
target_link_libraries(FakeBasketball PRIVATE glfw libglew_static)