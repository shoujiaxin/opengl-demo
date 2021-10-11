// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <algorithm>
#include <iostream>
#include <vector>

#include "buffer/array_buffer.h"
#include "buffer/vertex_array.h"
#include "camera/perspective_camera.h"
#include "control/control.h"
#include "glm/gtc/matrix_transform.hpp"
#include "shader/program.h"
#include "texture/texture.h"

#define SCR_WIDTH 1200
#define SCR_HEIGHT 800

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void HandleKeyboardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }
}

int main() {
  // 初始化 GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // 创建窗口
  const auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Demo", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  // 捕捉并隐藏光标
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // 初始化 GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // 开启深度测试
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // 开启混合
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  const auto program = Program("../shaders/vertex_shaders/depth_testing.vert",
                               "../shaders/fragment_shaders/blending.frag");
  // 使用 GL_TEXTURE0 纹理单元，渲染时将不同的纹理绑定到该单元
  program.SetUniform("texture1", 0);

  const auto cube_vertices = std::vector<float>{
      // ----- 位置 -----, --- 纹理坐标 ---
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f   //
  };
  const auto plane_vertices = std::vector<float>{
      // ----- 位置 -----, --- 纹理坐标 ---
      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //
      -5.0f, -0.5f, 5.0f,  0.0f, 0.0f,  //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,  //

      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,  //
      5.0f,  -0.5f, -5.0f, 2.0f, 2.0f   //
  };
  const auto transparent_vertices = std::vector<float>{
      // ---- 位置 ----, --- 纹理坐标 ---
      0.0f, 0.5f,  0.0f, 0.0f, 1.0f,  //
      0.0f, -0.5f, 0.0f, 0.0f, 0.0f,  //
      1.0f, -0.5f, 0.0f, 1.0f, 0.0f,  //

      0.0f, 0.5f,  0.0f, 0.0f, 1.0f,  //
      1.0f, -0.5f, 0.0f, 1.0f, 0.0f,  //
      1.0f, 0.5f,  0.0f, 1.0f, 1.0f   //
  };
  auto vegetation_positions = std::vector<glm::vec3>{{-1.5f, 0.0f, -0.48f},
                                                     {1.5f, 0.0f, 0.51f},
                                                     {0.0f, 0.0f, 0.7f},
                                                     {-0.3f, 0.0f, -2.3f},
                                                     {0.5f, 0.0f, -0.6f}};

  // 立方体
  const auto cube_vao = VertexArray();
  const auto cube_vbo = ArrayBuffer(cube_vertices);
  cube_vao.Bind(cube_vbo);
  cube_vao.SetAttribute(0, 3, 5 * sizeof(float), 0);
  cube_vao.SetAttribute(1, 2, 5 * sizeof(float), 3 * sizeof(float));

  // 平面
  const auto plane_vao = VertexArray();
  const auto plane_vbo = ArrayBuffer(plane_vertices);
  plane_vao.Bind(plane_vbo);
  plane_vao.SetAttribute(0, 3, 5 * sizeof(float), 0);
  plane_vao.SetAttribute(1, 2, 5 * sizeof(float), 3 * sizeof(float));

  // 透明纹理
  const auto vegetation_vao = VertexArray();
  const auto vegetation_vbo = ArrayBuffer(transparent_vertices);
  vegetation_vao.Bind(vegetation_vbo);
  vegetation_vao.SetAttribute(0, 3, 5 * sizeof(float), 0);
  vegetation_vao.SetAttribute(1, 2, 5 * sizeof(float), 3 * sizeof(float));

  const auto cube_texture = Texture("../assets/textures/marble.jpg");
  const auto floor_texture = Texture("../assets/textures/metal.png");
  floor_texture.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
  floor_texture.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);
  const auto grass_texture = Texture("../assets/textures/window.png");
  grass_texture.SetWrap(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  grass_texture.SetWrap(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // 相机
  auto camera = PerspectiveCamera(45.0f, static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
  camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
  auto control = Control(camera, window);

  // 渲染循环 (render loop)
  while (!glfwWindowShouldClose(window)) {
    // 输入
    HandleKeyboardInput(window);

    // 清除缓冲
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    control.Update();

    program.Use();
    program.SetUniform("view", camera.ViewMatrix());
    program.SetUniform("projection", camera.ProjectionMatrix());

    auto model_matrix = glm::mat4(1.0f);

    // 绘制立方体
    cube_vao.Bind();
    cube_texture.BindToUnit(0);
    model_matrix = glm::mat4(1.0f);
    // Y 轴加 0.001 偏移，避免 Z-fighting 问题
    program.SetUniform("model", glm::translate(model_matrix, glm::vec3(-1.0f, 0.001f, -1.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model_matrix = glm::mat4(1.0f);
    program.SetUniform("model", glm::translate(model_matrix, glm::vec3(2.0f, 0.001f, 0.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 绘制平面
    plane_vao.Bind();
    floor_texture.BindToUnit(0);
    program.SetUniform("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 排序透明物体，先绘制最远的
    std::sort(vegetation_positions.begin(), vegetation_positions.end(),
              [&](const auto& lhs, const auto& rhs) {
                return glm::length(camera.Position() - lhs) > glm::length(camera.Position() - rhs);
              });

    // 绘制透明纹理
    vegetation_vao.Bind();
    grass_texture.BindToUnit(0);
    for (const auto& position : vegetation_positions) {
      model_matrix = glm::mat4(1.0f);
      model_matrix = glm::translate(model_matrix, position);
      program.SetUniform("model", model_matrix);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
