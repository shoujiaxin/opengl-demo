// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <iostream>

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

  // 顶点着色器
  const char* vertex_shader_source =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main() {\n"
      "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\n";
  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertex_shader);
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cerr << info_log << std::endl;
  }

  // 片段着色器
  const char* fragment_shader_source =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main() {\n"
      "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
      "}\n";
  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cerr << info_log << std::endl;
  }

  // 着色器程序
  auto shader_program = glad_glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_program, 512, nullptr, info_log);
    std::cerr << info_log << std::endl;
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // 顶点数据
  float vertices[] = {
      0.5f,  0.5f,  0.0f,  // 右上角
      0.5f,  -0.5f, 0.0f,  // 右下角
      -0.5f, -0.5f, 0.0f,  // 左下角
      -0.5f, 0.5f,  0.0f   // 左上角
  };
  unsigned int indices[] = {
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解绑顶点缓冲对象
  glBindVertexArray(0);              // 解绑顶点数组对象

  // 线框模式
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // 默认模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // 渲染循环 (render loop)
  while (!glfwWindowShouldClose(window)) {
    HandleInput(window);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_object);
    //    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);  // 交换颜色缓冲
    glfwPollEvents();         // 检查触发事件（键盘输入、鼠标移动等）
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}
