cmake_minimum_required(VERSION 3.12)
project(FakeBasketball)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} C:/dev/CMakeRust/cmake)

file(GLOB_RECURSE SOURCE_FILES
        main.cpp
        OpenGL.cpp
        RigidBodies/*.cpp
        Collideable/*.cpp
        InputHandler/*.cpp
)

enable_language(Rust)
include(CmakeCargo)

add_subdirectory(glew)
add_subdirectory(glfw)
add_subdirectory(server)

include_directories(glfw/include)
include_directories(glew/include)
include_directories(glm)
include_directories(server)

add_executable(FakeBasketball ${SOURCE_FILES})

target_link_libraries(FakeBasketball PRIVATE basketball_networking ws2_32 userenv)
#include(C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(Freetype REQUIRED)
target_link_libraries(FakeBasketball PRIVATE Freetype::Freetype)
target_link_libraries(FakeBasketball PRIVATE glfw libglew_static)