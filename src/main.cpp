// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on
#include <cmath>
#include <iostream>

#include "program.h"
#include "shader.h"
#include "texture.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void HandleInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
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
  auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  // 初始化 GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  const auto program = Program(VertexShader("../shader/vertex_shader/texture.vert"),
                               FragmentShader("../shader/fragment_shader/texture_mix.frag"));
  program.Use();
  program.SetUniform("texture1", 0);
  program.SetUniform("texture2", 1);
  const auto texture1 = Texture("../resource/texture/container.jpg");
  const auto texture2 = Texture("../resource/texture/awesomeface.png");

  // 顶点数据
  const float vertices[] = {
      // ----- 位置 -----, ----- 颜色 -----, --- 纹理坐标 ---
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上角
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下角
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下角
      -0.5f, 0.5f,  0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f   // 左上角
  };
  const unsigned int indices[] = {
      0, 1, 3,  // 第一个三角形
      1, 2, 3   // 第二个三角形
  };

  unsigned int vertex_array_object;    // 顶点数组对象
  unsigned int vertex_buffer_object;   // 顶点缓冲对象
  unsigned int element_buffer_object;  // 索引缓冲对象
  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &element_buffer_object);

  glBindVertexArray(vertex_array_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  constexpr auto stride = 8 * sizeof(float);
  // 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
  glEnableVertexAttribArray(0);
  // 颜色属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 纹理属性
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解绑顶点缓冲对象
  glBindVertexArray(0);              // 解绑顶点数组对象

  // 线框模式
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // 默认模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // 渲染循环 (render loop)
  while (!glfwWindowShouldClose(window)) {
    // 输入
    HandleInput(window);

    // 清除颜色缓冲
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 激活着色器程序
    program.Use();

    //    const auto time_value = glfwGetTime();
    //    const auto green_value = static_cast<float>(sin(time_value / 2.0f)) + 0.5f;
    //    const auto vertex_color_location = glGetUniformLocation(shader_program, "ourColor");
    //    glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    texture1.Bind();
    glActiveTexture(GL_TEXTURE1);
    texture2.Bind();

    // 绘制图形
    glBindVertexArray(vertex_array_object);
    //    glDrawArrays(GL_TRIANGLES, 0, 3);  // 三角形
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // 矩形

    glfwSwapBuffers(window);  // 交换颜色缓冲
    glfwPollEvents();         // 检查触发事件（键盘输入、鼠标移动等）
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);

  glfwTerminate();
  return 0;
}
