// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <iostream>
#include <vector>

#include "buffer/array_buffer.h"
#include "buffer/vertex_array.h"
#include "camera/perspective_camera.h"
#include "control/control.h"
#include "glm/gtc/matrix_transform.hpp"
#include "model/model.h"
#include "shader/geometry_shader.h"
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
  glDepthFunc(GL_LEQUAL);

  const auto vertex_shader = VertexShader("../shaders/vertex_shaders/explode.vert");
  const auto geometry_shader = GeometryShader("../shaders/geometry_shaders/explode.geom");
  const auto fragment_shader = FragmentShader("../shaders/fragment_shaders/model_reflection.frag");
  const auto program = Program();
  program.AttachShader(vertex_shader);
  program.AttachShader(geometry_shader);
  program.AttachShader(fragment_shader);
  program.Link();
  program.SetUniform("ourColor", glm::vec3(0.0f, 1.0f, 0.0f));

  //  const auto points = std::vector<float>{
  //      0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  //
  //      -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  //
  //      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f,  //
  //      -0.5f, -0.5f, 1.0f, 1.0f, 0.0f   //
  //  };
  //
  //  const auto vao = VertexArray();
  //  const auto vbo = ArrayBuffer(points);
  //  vao.Bind(vbo);
  //  vao.SetAttribute(0, 2, 5 * sizeof(float), 0);
  //  vao.SetAttribute(1, 3, 5 * sizeof(float), 2 * sizeof(float));

  // 模型
  const auto model = Model("../assets/models/nanosuit/nanosuit.obj");

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

    program.SetUniform("view", camera.ViewMatrix());
    program.SetUniform("projection", camera.ProjectionMatrix());
    program.SetUniform("viewPos", camera.Position());
    program.SetUniform("time", static_cast<float>(glfwGetTime()));

    auto model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(2.0f, -0.5f, 0.0f));
    model_matrix = glm::scale(model_matrix, glm::vec3(0.1f));
    program.SetUniform("model", model_matrix);
    model.Draw(program);

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
