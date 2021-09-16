// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <iostream>

#include "buffers.h"
#include "cameras.h"
#include "controls.h"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"
#include "shaders.h"
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

  // 开启面剔除
  glEnable(GL_CULL_FACE);
  //  glCullFace(GL_FRONT);  // 需要剔除的面
  //  glFrontFace(GL_CW);    // 需要剔除面的方向

  // 加载天空盒
  const auto skybox_texture = Texture({
      // 按照枚举顺序
      "../resource/texture/skybox/right.jpg",   //
      "../resource/texture/skybox/left.jpg",    //
      "../resource/texture/skybox/top.jpg",     //
      "../resource/texture/skybox/bottom.jpg",  //
      "../resource/texture/skybox/front.jpg",   //
      "../resource/texture/skybox/back.jpg"     //
  });
  const auto cube_texture = Texture("../resource/texture/container.jpg");

  // 天空盒顶点
  const auto skybox_vertices =
      std::vector<float>{-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                         1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                         -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                         -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                         1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                         1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                         -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                         1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                         -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                         1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                         -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                         1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  const auto cube_vertices = std::vector<float>{
      // ------ 位置 ------, ------ 法向量 ------
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,  //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,  //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,  //

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  //

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  //
      -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  //
      -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  //
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  //

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  //

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  //
      0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  //
      -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  //
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  //

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  //
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  //
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  //
      -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f   //
  };

  // 天空盒
  unsigned int skybox_vao;
  glGenVertexArrays(1, &skybox_vao);
  glBindVertexArray(skybox_vao);
  const auto skybox_vbo = ArrayBuffer(skybox_vertices);
  skybox_vbo.Bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glBindVertexArray(0);

  // 立方体
  unsigned int cube_vao;
  glGenVertexArrays(1, &cube_vao);
  glBindVertexArray(cube_vao);
  const auto cube_vbo = ArrayBuffer(cube_vertices);
  cube_vbo.Bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void*>(3 * sizeof(float)));
  glBindVertexArray(0);

  // 模型
  const auto model = Model("../resource/model/nanosuit/nanosuit.obj");

  const auto skybox_program = Program(VertexShader("../shader/vertex_shader/skybox.vert"),
                                      FragmentShader("../shader/fragment_shader/skybox.frag"));
  const auto program = Program(VertexShader("../shader/vertex_shader/reflection.vert"),
                               FragmentShader("../shader/fragment_shader/refraction.frag"));
  const auto model_program =
      Program(VertexShader("../shader/vertex_shader/multiple_lights.vert"),
              FragmentShader("../shader/fragment_shader/model_reflection.frag"));

  // 相机
  auto camera = PerspectiveCamera(45.0f, static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
  camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
  auto controls = Controls(camera, window);

  // 渲染循环 (render loop)
  while (!glfwWindowShouldClose(window)) {
    // 输入
    HandleKeyboardInput(window);

    // 清除缓冲
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    controls.Update();

    skybox_program.Use();
    // 移除矩阵位移部分
    skybox_program.SetUniform("view", glm::mat4(glm::mat3(camera.ViewMatrix())));
    skybox_program.SetUniform("projection", camera.ProjectionMatrix());

    program.Use();
    program.SetUniform("view", camera.ViewMatrix());
    program.SetUniform("model", glm::mat4(1.0f));
    program.SetUniform("projection", camera.ProjectionMatrix());
    program.SetUniform("cameraPos", camera.Position());

    // 绘制天空盒
    glDepthMask(GL_FALSE);
    skybox_program.Use();
    glBindVertexArray(skybox_vao);
    skybox_program.SetUniform("skybox", 0);
    glActiveTexture(GL_TEXTURE0);
    skybox_texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    // 绘制立方体
    program.Use();
    glBindVertexArray(cube_vao);
    //    cube_texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    auto model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(2.0f, -0.5f, 0.0f));
    model_matrix = glm::scale(model_matrix, glm::vec3(0.1f));
    model_program.Use();
    model_program.SetUniform("view", camera.ViewMatrix());
    model_program.SetUniform("model", model_matrix);
    model_program.SetUniform("projection", camera.ProjectionMatrix());
    model_program.SetUniform("viewPos", camera.Position());
    model_program.SetUniform("skybox", 1);
    glActiveTexture(GL_TEXTURE1);
    skybox_texture.Bind();
    model.Draw(model_program);

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &skybox_vao);
  glDeleteVertexArrays(1, &cube_vao);

  glfwTerminate();
  return 0;
}
