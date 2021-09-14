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

include_directories(include)

file(GLOB SOURCE_FILES
        src/controls.cpp
        src/framebuffer.cpp
        src/mesh.cpp
        src/model.cpp
        src/renderbuffer.cpp
        src/texture.cpp
        src/*/*.cpp
        )

add_executable(Demo src/main.cpp ${SOURCE_FILES})
target_link_libraries(Demo glfw glad assimp)

add_executable(DepthTesting src/depth_testing.cpp ${SOURCE_FILES})
target_link_libraries(DepthTesting glfw glad assimp)

add_executable(StencilTesting src/stencil_testing.cpp ${SOURCE_FILES})
target_link_libraries(StencilTesting glfw glad assimp)

add_executable(Blending src/blending.cpp ${SOURCE_FILES})
target_link_libraries(Blending glfw glad assimp)
