// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <iostream>
#include <memory>
#include <vector>

#include "buffer/array_buffer.h"
#include "buffer/vertex_array.h"
#include "shader/program.h"
#include "util/bezier_path.h"

#define SCR_WIDTH 1200
#define SCR_HEIGHT 800

BezierPath path;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void CursorPosCallback(GLFWwindow* window, double x, double y) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    auto width = 0;
    auto height = 0;
    glfwGetWindowSize(window, &width, &height);

    path.AddCurveTo({2 * x / width - 1, 1 - 2 * y / height},
                    {2 * x / width - 1, 1 - 2 * y / height});
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
  glfwSetCursorPosCallback(window, CursorPosCallback);

  // 初始化 GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_PROGRAM_POINT_SIZE);

  const auto paint_program =
      Program("../shaders/vertex_shaders/paint.vert", "../shaders/fragment_shaders/paint.frag");
  paint_program.SetUniform("size", 10.0f);

  // 轨迹点
  const auto vao = VertexArray();
  auto vbo = ArrayBuffer();
  vao.Bind(vbo);
  vao.SetAttribute(0, 2, 2 * sizeof(float), 0);

  // 默认模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  while (!glfwWindowShouldClose(window)) {
    // 清除缓冲
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto guard = BindGuard(vao);
    std::vector<float> vertices;
    for (const auto& p : path.GetPoints()) {
      vertices.emplace_back(p.x_);
      vertices.emplace_back(p.y_);
    }
    vbo.SetData(vertices);

    paint_program.Use();
    glDrawArrays(GL_POINTS, 0, static_cast<int>(vertices.size()) / 2);

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
