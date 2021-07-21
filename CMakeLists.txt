cmake_minimum_required(VERSION 3.19)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 11)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

include_directories(include)

add_executable(learn_opengl src/main.cpp src/glad.c src/program.cpp src/shader.cpp)

target_link_libraries(learn_opengl glfw)
