// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <iostream>
#include <vector>

#include "buffer/array_buffer.h"
#include "buffer/uniform_buffer.h"
#include "buffer/vertex_array.h"
#include "camera/perspective_camera.h"
#include "control/control.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader/program.h"

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

  const auto red_program = Program(VertexShader("../shaders/vertex_shaders/uniform_buffer.vert"),
                                   FragmentShader("../shaders/fragment_shaders/color2.frag"));
  red_program.Use();
  red_program.SetUniform("ourColor", glm::vec3(1.0f, 0.0f, 0.0f));
  red_program.BindUniformBlock("Matrices", 0);
  const auto green_program = Program(VertexShader("../shaders/vertex_shaders/uniform_buffer.vert"),
                                     FragmentShader("../shaders/fragment_shaders/color2.frag"));
  green_program.Use();
  green_program.SetUniform("ourColor", glm::vec3(0.0f, 1.0f, 0.0f));
  green_program.BindUniformBlock("Matrices", 0);
  const auto blue_program = Program(VertexShader("../shaders/vertex_shaders/uniform_buffer.vert"),
                                    FragmentShader("../shaders/fragment_shaders/color2.frag"));
  blue_program.Use();
  blue_program.SetUniform("ourColor", glm::vec3(0.0f, 0.0f, 1.0f));
  blue_program.BindUniformBlock("Matrices", 0);
  const auto yellow_program = Program(VertexShader("../shaders/vertex_shaders/uniform_buffer.vert"),
                                      FragmentShader("../shaders/fragment_shaders/color2.frag"));
  yellow_program.Use();
  yellow_program.SetUniform("ourColor", glm::vec3(1.0f, 1.0f, 0.0f));
  yellow_program.BindUniformBlock("Matrices", 0);

  const auto ubo = UniformBuffer(2 * sizeof(glm::mat4));
  ubo.Bind(0, 0, ubo.Size());

  const auto cube_vertices = std::vector<float>{
      // ----- 位置 -----
      -0.5f, -0.5f, -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  0.5f,  -0.5f,  //
      0.5f,  0.5f,  -0.5f,  //
      -0.5f, 0.5f,  -0.5f,  //
      -0.5f, -0.5f, -0.5f,  //

      -0.5f, -0.5f, 0.5f,  //
      0.5f,  -0.5f, 0.5f,  //
      0.5f,  0.5f,  0.5f,  //
      0.5f,  0.5f,  0.5f,  //
      -0.5f, 0.5f,  0.5f,  //
      -0.5f, -0.5f, 0.5f,  //

      -0.5f, 0.5f,  0.5f,   //
      -0.5f, 0.5f,  -0.5f,  //
      -0.5f, -0.5f, -0.5f,  //
      -0.5f, -0.5f, -0.5f,  //
      -0.5f, -0.5f, 0.5f,   //
      -0.5f, 0.5f,  0.5f,   //

      0.5f,  0.5f,  0.5f,   //
      0.5f,  0.5f,  -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  -0.5f, 0.5f,   //
      0.5f,  0.5f,  0.5f,   //

      -0.5f, -0.5f, -0.5f,  //
      0.5f,  -0.5f, -0.5f,  //
      0.5f,  -0.5f, 0.5f,   //
      0.5f,  -0.5f, 0.5f,   //
      -0.5f, -0.5f, 0.5f,   //
      -0.5f, -0.5f, -0.5f,  //

      -0.5f, 0.5f,  -0.5f,  //
      0.5f,  0.5f,  -0.5f,  //
      0.5f,  0.5f,  0.5f,   //
      0.5f,  0.5f,  0.5f,   //
      -0.5f, 0.5f,  0.5f,   //
      -0.5f, 0.5f,  -0.5f,  //
  };

  // 立方体
  const auto cube_vao = VertexArray();
  const auto cube_vbo = ArrayBuffer(cube_vertices);
  cube_vao.Bind(cube_vbo);
  cube_vao.SetAttribute(0, 3, 3 * sizeof(float), 0);

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

    ubo.SetSubData(glm::value_ptr(camera.ViewMatrix()), sizeof(glm::mat4), 0);
    ubo.SetSubData(glm::value_ptr(camera.ProjectionMatrix()), sizeof(glm::mat4), sizeof(glm::mat4));

    cube_vao.Bind();

    auto model_matrix = glm::mat4(1.0f);

    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(-0.75f, 0.75f, 0.0f));
    red_program.Use();
    red_program.SetUniform("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(0.75f, 0.75f, 0.0f));
    green_program.Use();
    green_program.SetUniform("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(-0.75f, -0.75f, 0.0f));
    blue_program.Use();
    blue_program.SetUniform("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(0.75f, -0.75f, 0.0f));
    yellow_program.Use();
    yellow_program.SetUniform("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
