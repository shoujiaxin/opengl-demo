cmake_minimum_required(VERSION 3.19)
project(opengl-demo)

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

# spdlog
add_subdirectory(third_party/spdlog)

include_directories(include include/*)

file(GLOB SOURCE_FILES
        src/*.cpp
        src/*/*.cpp
        )

add_executable(Demo examples/main.cpp ${SOURCE_FILES})
target_link_libraries(Demo glfw glad assimp spdlog)

add_executable(DepthTesting examples/depth_testing.cpp ${SOURCE_FILES})
target_link_libraries(DepthTesting glfw glad assimp spdlog)

add_executable(StencilTesting examples/stencil_testing.cpp ${SOURCE_FILES})
target_link_libraries(StencilTesting glfw glad assimp spdlog)

add_executable(Blending examples/blending.cpp ${SOURCE_FILES})
target_link_libraries(Blending glfw glad assimp spdlog)

add_executable(CubeMap examples/cube_map.cpp ${SOURCE_FILES})
target_link_libraries(CubeMap glfw glad assimp spdlog)

add_executable(AdvancedGLSL examples/advanced_glsl.cpp ${SOURCE_FILES})
target_link_libraries(AdvancedGLSL glfw glad assimp spdlog)

add_executable(GeometryShader examples/geometry_shader_demo.cpp ${SOURCE_FILES})
target_link_libraries(GeometryShader glfw glad assimp spdlog)
