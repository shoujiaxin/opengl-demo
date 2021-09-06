// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "camera.h"
#include "controls.h"
#include "glm/gtc/matrix_transform.hpp"
#include "light.h"
#include "program.h"
#include "shader.h"
#include "texture.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

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

  // 初始化 GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // 顶点数据
  //  const float vertices[] = {
  //      // ----- 位置 -----, ----- 颜色 -----, --- 纹理坐标 ---
  //      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上角
  //      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下角
  //      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下角
  //      -0.5f, 0.5f,  0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f   // 左上角
  //  };
  // 立方体：6 个面 x 每个面 2 个三角形 x 每个三角形 3 个顶点 = 36 个顶点
  constexpr float vertices[] = {
      // ----- 位置 -----, ------ 法向量 ------, --- 纹理坐标 ---
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,  //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  //

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  //

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  //
      -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,  //
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  //

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,  //

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f   //
  };
  constexpr unsigned int indices[] = {
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

  constexpr auto stride = 8 * sizeof(float);  // 步长
  // 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
  glEnableVertexAttribArray(0);
  // 颜色属性
  //  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  //  glEnableVertexAttribArray(1);
  // 纹理属性
  //  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  //  glEnableVertexAttribArray(2);
  // 法向量属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 光照贴图纹理坐标属性
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解绑顶点缓冲对象
  glBindVertexArray(0);              // 解绑顶点数组对象

  // 着色器程序
  const auto program = Program(VertexShader("../shader/vertex_shader/lighting_map.vert"),
                               FragmentShader("../shader/fragment_shader/lighting_map.frag"));
  program.Use();
  program.SetUniform("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
  program.SetUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
  program.SetUniform("material.specular", glm::vec3(0.0f));
  program.SetUniform("material.shininess", 32.0f);

  // 加载纹理
  //  program.SetUniform("texture1", 0);
  //  glActiveTexture(GL_TEXTURE0);
  //  const auto texture1 = Texture("../resource/texture/container.jpg");
  //  program.SetUniform("texture2", 1);
  //  glActiveTexture(GL_TEXTURE1);
  //  const auto texture2 = Texture("../resource/texture/awesomeface.png");

  // 加载光照贴图
  program.SetUniform("material.diffuse", 0);
  glActiveTexture(GL_TEXTURE0);
  const auto diffuse_map = Texture("../resource/lighting_map/container2.png");
  program.SetUniform("material.specular", 1);
  glActiveTexture(GL_TEXTURE1);
  const auto specular_map = Texture("../resource/lighting_map/container2_specular.png");

  // 光照
  unsigned int light_vertex_array_object;
  glGenVertexArrays(1, &light_vertex_array_object);
  glBindVertexArray(light_vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  const auto light_program = Program(VertexShader("../shader/vertex_shader/lighting.vert"),
                                     FragmentShader("../shader/fragment_shader/light.frag"));
  auto ambient_light = AmbientLight(Color::kWhite, 0.2);
  auto diffuse_light = DiffuseLight(Color::kWhite, 0.5);
  diffuse_light.SetPosition(glm::vec3(1.2f, 1.0f, 2.0f));
  auto specular_light = SpecularLight(Color::kWhite, 1.0);
  specular_light.SetPosition(diffuse_light.Position());

  // 线框模式
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // 默认模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // 开启深度测试
  glEnable(GL_DEPTH_TEST);

  // 相机
  auto camera = PerspectiveCamera(45.0f, static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
  camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
  auto controls = Controls(camera, window);

  // 渲染循环 (render loop)
  while (!glfwWindowShouldClose(window)) {
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

    // 改变光照颜色
    const auto current_time = glfwGetTime();
    const auto light_color =
        glm::vec3(sin(current_time * 2.0f), sin(current_time * 0.7f), sin(current_time * 1.3f));
    //    diffuse_light.SetColor(light_color);
    //    diffuse_light.SetIntensity(0.5f);
    //    ambient_light.SetColor(light_color);
    //    ambient_light.SetIntensity(0.2f);

    // 移动相机
    controls.Update();

    // 灯光位置
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, diffuse_light.Position());
    model = glm::scale(model, glm::vec3(0.2f));
    light_program.Use();
    light_program.SetUniform("model", model);
    light_program.SetUniform("view", camera.ViewMatrix());
    light_program.SetUniform("projection", camera.ProjectionMatrix());

    // 绘制灯立方体
    glBindVertexArray(light_vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 激活着色器程序
    program.Use();
    //    program.SetUniformMatrix4fv("model", glm::value_ptr(model_matrix));
    program.SetUniform("view", camera.ViewMatrix());
    program.SetUniform("projection", camera.ProjectionMatrix());
    program.SetUniform("lightPos", diffuse_light.Position());
    program.SetUniform("light.ambient", ambient_light.Color());
    program.SetUniform("light.diffuse", diffuse_light.Color());
    program.SetUniform("light.specular", specular_light.Color());

    //    const auto time_value = glfwGetTime();
    //    const auto green_value = static_cast<float>(sin(time_value / 2.0f)) + 0.5f;
    //    const auto vertex_color_location = glGetUniformLocation(shader_program, "ourColor");
    //    glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

    // 绘制图形
    glBindVertexArray(vertex_array_object);
    //    glDrawArrays(GL_TRIANGLES, 0, 3);  // 三角形
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // 矩形
    //    glDrawArrays(GL_TRIANGLES, 0, 36);  // 立方体
    //    std::default_random_engine e;
    //    std::uniform_int_distribution<unsigned> u(0, cube_positions.size());
    //    for (const auto& position : cube_positions) {
    //      auto model_matrix = glm::mat4(1.0f);
    //      model_matrix = glm::translate(model_matrix, position);
    //      model_matrix =
    //          glm::rotate(model_matrix, glm::radians(20.0f * u(e)), glm::vec3(1.0f, 0.3f, 0.5f));
    //      program.SetUniform("model", model_matrix);
    //      glDrawArrays(GL_TRIANGLES, 0, 36);
    //    }
    program.SetUniform("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);  // 交换颜色缓冲
    glfwPollEvents();         // 检查触发事件（键盘输入、鼠标移动等）
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);

  glfwTerminate();
  return 0;
}
