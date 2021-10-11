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
#include "model/model.h"
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
  glDepthFunc(GL_LEQUAL);

  // 开启面剔除
  glEnable(GL_CULL_FACE);
  //  glCullFace(GL_FRONT);  // 需要剔除的面
  //  glFrontFace(GL_CW);    // 需要剔除面的方向

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
  const auto skybox_vao = VertexArray();
  const auto skybox_vbo = ArrayBuffer(skybox_vertices);
  skybox_vao.Bind(skybox_vbo);
  skybox_vao.SetAttribute(0, 3, 3 * sizeof(float), 0);

  // 立方体
  const auto cube_vao = VertexArray();
  const auto cube_vbo = ArrayBuffer(cube_vertices);
  cube_vao.Bind(cube_vbo);
  cube_vao.SetAttribute(0, 3, 6 * sizeof(float), 0);
  cube_vao.SetAttribute(1, 3, 6 * sizeof(float), 3 * sizeof(float));

  // 着色器程序
  const auto skybox_program =
      Program("../shaders/vertex_shaders/skybox.vert", "../shaders/fragment_shaders/skybox.frag");
  skybox_program.SetUniform("skybox", 0);
  const auto cube_program = Program("../shaders/vertex_shaders/reflection.vert",
                                    "../shaders/fragment_shaders/refraction.frag");
  cube_program.SetUniform("skybox", 0);
  const auto model_program = Program("../shaders/vertex_shaders/multiple_lights.vert",
                                     "../shaders/fragment_shaders/model_reflection.frag");
  model_program.SetUniform("skybox", 0);

  // 纹理
  const auto skybox_texture = Texture({
      // 按照枚举顺序
      "../assets/textures/skybox/right.jpg",   //
      "../assets/textures/skybox/left.jpg",    //
      "../assets/textures/skybox/top.jpg",     //
      "../assets/textures/skybox/bottom.jpg",  //
      "../assets/textures/skybox/front.jpg",   //
      "../assets/textures/skybox/back.jpg"     //
  });
  const auto cube_texture = Texture("../assets/textures/container.jpg");

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

    // 移除矩阵位移部分
    skybox_program.SetUniform("view", glm::mat4(glm::mat3(camera.ViewMatrix())));
    skybox_program.SetUniform("projection", camera.ProjectionMatrix());

    cube_program.SetUniform("view", camera.ViewMatrix());
    cube_program.SetUniform("model", glm::mat4(1.0f));
    cube_program.SetUniform("projection", camera.ProjectionMatrix());
    cube_program.SetUniform("cameraPos", camera.Position());

    model_program.SetUniform("view", camera.ViewMatrix());
    model_program.SetUniform("projection", camera.ProjectionMatrix());
    model_program.SetUniform("viewPos", camera.Position());

    // 绑定纹理
    skybox_texture.BindToUnit(0);

    // 绘制天空盒
    glDepthMask(GL_FALSE);
    skybox_program.Use();
    skybox_vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    // 绘制立方体
    cube_program.Use();
    cube_vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 绘制模型
    auto model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(2.0f, -0.5f, 0.0f));
    model_matrix = glm::scale(model_matrix, glm::vec3(0.1f));
    model_program.SetUniform("model", model_matrix);
    model.Draw(model_program);

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
