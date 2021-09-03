// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "program.h"
#include "shader.h"
#include "texture.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define CAMERA_FRONT glm::vec3(0.0f, 0.0f, -1.0f)
#define CAMERA_UP glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_RIGHT glm::vec3(1.0f, 0.0f, 0.0f)

auto last_frame_time = 0.0f;  // 上一帧的时间
auto delta_time = 0.0f;       // 当前帧与上一帧的时间差

auto first_cursor = true;
auto last_cursor_position = glm::vec2(0.5f, 0.5f);

auto camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
auto camera_front = CAMERA_FRONT;
auto camera_pitch = 0.0f;
auto camera_yaw = -90.0f;

auto fov = 45.0f;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void HandleKeyboardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }

  const auto camera_move_speed = 2.5f * delta_time;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera_position += camera_move_speed * camera_front;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera_position -= camera_move_speed * camera_front;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera_position -= camera_move_speed * glm::normalize(glm::cross(camera_front, CAMERA_UP));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera_position += camera_move_speed * glm::normalize(glm::cross(camera_front, CAMERA_UP));
  }
}

void HandleCursorMove(GLFWwindow* window, double x, double y) {
  const auto normalized_x = x / SCR_WIDTH;
  const auto normalized_y = y / SCR_HEIGHT;
  if (first_cursor) {
    first_cursor = false;
    last_cursor_position.x = normalized_x;
    last_cursor_position.y = normalized_y;
  }

  constexpr auto sensitivity = 100.0f;
  const auto offset_x = sensitivity * (normalized_x - last_cursor_position.x);
  const auto offset_y = sensitivity * (last_cursor_position.y - normalized_y);

  camera_yaw += offset_x;
  camera_pitch += offset_y;

  auto target = glm::vec3(0.0f, 0.0f, 0.0f);
  target.x = cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
  target.y = sin(glm::radians(camera_pitch));
  target.z = sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
  camera_front = glm::normalize(target);

  last_cursor_position.x = normalized_x;
  last_cursor_position.y = normalized_y;
}

void HandleScroll(GLFWwindow* window, double x, double y) {
  fov -= y;
  fov = std::min(fov, 45.0f);
  fov = std::max(fov, 1.0f);
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
  // 捕捉并隐藏光标
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, HandleCursorMove);
  glfwSetScrollCallback(window, HandleScroll);

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

  glActiveTexture(GL_TEXTURE0);
  const auto texture1 = Texture("../resource/texture/container.jpg");
  glActiveTexture(GL_TEXTURE1);
  const auto texture2 = Texture("../resource/texture/awesomeface.png");

  // 顶点数据
  //  const float vertices[] = {
  //      // ----- 位置 -----, ----- 颜色 -----, --- 纹理坐标 ---
  //      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上角
  //      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下角
  //      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下角
  //      -0.5f, 0.5f,  0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f   // 左上角
  //  };
  const float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
                            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                            0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
                            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                            -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
  const unsigned int indices[] = {
      0, 1, 3,  // 第一个三角形
      1, 2, 3   // 第二个三角形
  };
  const auto cube_positions =
      std::vector<glm::vec3>{glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                             glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                             glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

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

  constexpr auto stride = 5 * sizeof(float);
  // 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
  glEnableVertexAttribArray(0);
  // 颜色属性
  //  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  //  glEnableVertexAttribArray(1);
  // 纹理属性
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解绑顶点缓冲对象
  glBindVertexArray(0);              // 解绑顶点数组对象

  // 线框模式
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // 默认模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // 开启深度测试
  glEnable(GL_DEPTH_TEST);

  // 相机
  auto camera = Camera(camera_position);

  // 渲染循环 (render loop)
  while (!glfwWindowShouldClose(window)) {
    // 计算两帧之间的时间差
    const auto current_time = glfwGetTime();
    delta_time = current_time - last_frame_time;
    last_frame_time = current_time;

    // 输入
    HandleKeyboardInput(window);

    // 清除颜色缓冲
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 模型矩阵
    //    auto model_matrix = glm::mat4(1.0f);
    //    model_matrix =
    //        glm::rotate(model_matrix, static_cast<float>(glfwGetTime()) * glm::radians(-55.0f),
    //                    glm::vec3(0.5f, 1.0f, 0.0f));

    // 观察矩阵
    auto view_matrix = glm::mat4(1.0f);
    view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));

    // 投影矩阵
    auto projection_matrix = glm::perspective(
        glm::radians(fov), static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);

    // 移动相机
    //    const auto radius = 10.0f;
    //    const auto camera_x = sin(glfwGetTime()) * radius;
    //    const auto camera_z = cos(glfwGetTime()) * radius;
    camera.SetPosition(camera_position);
    camera.LookAt(camera_position + camera_front);

    // 激活着色器程序
    program.Use();
    program.SetUniformMatrix4fv("view", camera.ViewMatrix());
    //    program.SetUniformMatrix4fv("model", glm::value_ptr(model_matrix));
    program.SetUniformMatrix4fv("projection", glm::value_ptr(projection_matrix));

    //    const auto time_value = glfwGetTime();
    //    const auto green_value = static_cast<float>(sin(time_value / 2.0f)) + 0.5f;
    //    const auto vertex_color_location = glGetUniformLocation(shader_program, "ourColor");
    //    glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

    // 绘制图形
    glBindVertexArray(vertex_array_object);
    //    glDrawArrays(GL_TRIANGLES, 0, 3);  // 三角形
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // 矩形
    //    glDrawArrays(GL_TRIANGLES, 0, 36);  // 立方体
    std::default_random_engine e;
    std::uniform_int_distribution<unsigned> u(0, cube_positions.size());
    for (const auto& position : cube_positions) {
      auto model_matrix = glm::mat4(1.0f);
      model_matrix = glm::translate(model_matrix, position);
      model_matrix =
          glm::rotate(model_matrix, glm::radians(20.0f * u(e)), glm::vec3(1.0f, 0.3f, 0.5f));
      program.SetUniformMatrix4fv("model", glm::value_ptr(model_matrix));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);  // 交换颜色缓冲
    glfwPollEvents();         // 检查触发事件（键盘输入、鼠标移动等）
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);

  glfwTerminate();
  return 0;
}
