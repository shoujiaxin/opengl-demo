cmake_minimum_required(VERSION 3.19)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# GLAD
add_subdirectory(third_party/glad)
include_directories(third_party/glad/include)

# GLM
include_directories(third_party/glm)

# stb
include_directories(third_party/stb)

# assimp
add_subdirectory(third_party/assimp)
include_directories(third_party/assimp/include)

include_directories(include)

file(GLOB SOURCE_FILES src/*.cpp)

add_executable(learn_opengl ${SOURCE_FILES})

target_link_libraries(learn_opengl glfw glad assimp)
