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
  const auto point_light_positions =
      std::vector<glm::vec3>{glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
                             glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

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
  const auto program = Program(VertexShader("../shader/vertex_shader/multiple_lights.vert"),
                               FragmentShader("../shader/fragment_shader/multiple_lights.frag"));
  program.Use();
  program.SetUniform("material.shininess", 32.0f);

  // 加载光照贴图
  program.SetUniform("material.diffuse", 0);
  glActiveTexture(GL_TEXTURE0);
  const auto diffuse_map = Texture("../resource/lighting_map/container2.png");
  program.SetUniform("material.specular", 1);
  glActiveTexture(GL_TEXTURE1);
  const auto specular_map = Texture("../resource/lighting_map/container2_specular.png");
  //  program.SetUniform("material.emission", 2);
  //  glActiveTexture(GL_TEXTURE2);
  //  const auto emission_map = Texture("../resource/lighting_map/matrix.jpg");

  // 光照
  unsigned int light_vertex_array_object;
  glGenVertexArrays(1, &light_vertex_array_object);
  glBindVertexArray(light_vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  const auto light_cube_program = Program(VertexShader("../shader/vertex_shader/lighting.vert"),
                                          FragmentShader("../shader/fragment_shader/light.frag"));

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

    // 移动相机
    controls.Update();

    // 灯光位置
    light_cube_program.Use();
    light_cube_program.SetUniform("view", camera.ViewMatrix());
    light_cube_program.SetUniform("projection", camera.ProjectionMatrix());
    for (const auto& position : point_light_positions) {
      auto model_matrix = glm::mat4(1.0f);
      model_matrix = glm::translate(model_matrix, position);
      model_matrix = glm::scale(model_matrix, glm::vec3(0.2f));
      light_cube_program.SetUniform("model", model_matrix);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // 绘制灯立方体
    glBindVertexArray(light_vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 激活着色器程序
    program.Use();
    //    program.SetUniformMatrix4fv("model", glm::value_ptr(model_matrix));
    program.SetUniform("view", camera.ViewMatrix());
    program.SetUniform("projection", camera.ProjectionMatrix());

    program.SetUniform("directionalLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    program.SetUniform("directionalLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    program.SetUniform("directionalLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    program.SetUniform("directionalLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    program.SetUniform("pointLights[0].position", point_light_positions[0]);
    program.SetUniform("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    program.SetUniform("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    program.SetUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    program.SetUniform("pointLights[0].constant", 1.0f);
    program.SetUniform("pointLights[0].linear", 0.09f);
    program.SetUniform("pointLights[0].quadratic", 0.032f);

    program.SetUniform("pointLights[1].position", point_light_positions[1]);
    program.SetUniform("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    program.SetUniform("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    program.SetUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    program.SetUniform("pointLights[1].constant", 1.0f);
    program.SetUniform("pointLights[1].linear", 0.09f);
    program.SetUniform("pointLights[1].quadratic", 0.032f);

    program.SetUniform("pointLights[2].position", point_light_positions[2]);
    program.SetUniform("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    program.SetUniform("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    program.SetUniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    program.SetUniform("pointLights[2].constant", 1.0f);
    program.SetUniform("pointLights[2].linear", 0.09f);
    program.SetUniform("pointLights[2].quadratic", 0.032f);

    program.SetUniform("pointLights[3].position", point_light_positions[3]);
    program.SetUniform("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    program.SetUniform("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    program.SetUniform("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    program.SetUniform("pointLights[3].constant", 1.0f);
    program.SetUniform("pointLights[3].linear", 0.09f);
    program.SetUniform("pointLights[3].quadratic", 0.032f);

    program.SetUniform("spotlight.position", camera.Position());
    program.SetUniform("spotlight.direction", camera.Front());
    program.SetUniform("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    program.SetUniform("spotlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    program.SetUniform("spotlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    program.SetUniform("spotlight.constant", 1.0f);
    program.SetUniform("spotlight.linear", 0.09f);
    program.SetUniform("spotlight.quadratic", 0.032f);
    program.SetUniform("spotlight.innerCutOff", glm::cos(glm::radians(12.5f)));
    program.SetUniform("spotlight.outerCutOff", glm::cos(glm::radians(15.0f)));

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
      program.SetUniform("model", model_matrix);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    //    program.SetUniform("model", glm::mat4(1.0f));
    //    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);  // 交换颜色缓冲
    glfwPollEvents();         // 检查触发事件（键盘输入、鼠标移动等）
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);

  glfwTerminate();
  return 0;
}
