// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <iostream>

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

  // 开启模板测试
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 1, 0xff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  const auto program = Program("../shaders/vertex_shaders/depth_testing.vert",
                               "../shaders/fragment_shaders/depth_testing.frag");
  program.SetUniform("texture1", 0);
  const auto outline_program = Program("../shaders/vertex_shaders/depth_testing.vert",
                                       "../shaders/fragment_shaders/light.frag");

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
  // 立方体位置，Y 轴加 0.001 偏移，避免 Z-fighting 问题
  const auto cube_positions = std::vector<glm::vec3>{
      {-1.0f, 0.001f, -1.0f},  //
      {2.0f, 0.001f, 0.0f}     //
  };

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

  const auto cube_texture = Texture("../assets/textures/marble.jpg");
  const auto floor_texture = Texture("../assets/textures/metal.png");
  floor_texture.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
  floor_texture.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    control.Update();

    program.SetUniform("view", camera.ViewMatrix());
    program.SetUniform("projection", camera.ProjectionMatrix());
    outline_program.SetUniform("view", camera.ViewMatrix());
    outline_program.SetUniform("projection", camera.ProjectionMatrix());

    auto model_matrix = glm::mat4(1.0f);

    glStencilMask(0x00);
    program.Use();

    // 绘制平面
    plane_vao.Bind();
    floor_texture.BindToUnit(0);
    program.SetUniform("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);

    // 绘制立方体
    cube_vao.Bind();
    cube_texture.BindToUnit(0);
    for (const auto& position : cube_positions) {
      model_matrix = glm::mat4(1.0f);
      program.SetUniform("model", glm::translate(model_matrix, position));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);  // 禁用写入模板缓冲
    glDisable(GL_DEPTH_TEST);
    outline_program.Use();

    // 绘制轮廓
    cube_vao.Bind();
    constexpr auto scale = 1.1f;
    for (const auto& position : cube_positions) {
      model_matrix = glm::mat4(1.0f);
      model_matrix = glm::translate(model_matrix, position);
      model_matrix = glm::scale(model_matrix, glm::vec3(scale, scale, scale));
      outline_program.SetUniform("model", model_matrix);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glStencilMask(0xff);
    glStencilFunc(GL_ALWAYS, 0, 0xff);
    glEnable(GL_DEPTH_TEST);

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
