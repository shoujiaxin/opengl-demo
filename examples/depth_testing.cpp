// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <iostream>
#include <vector>

#include "buffer/array_buffer.h"
#include "buffer/framebuffer.h"
#include "buffer/vertex_array.h"
#include "camera/perspective_camera.h"
#include "control/control.h"
#include "glm/gtc/matrix_transform.hpp"
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
  glDepthFunc(GL_LESS);

  // 开启面剔除
  glEnable(GL_CULL_FACE);
  //  glCullFace(GL_FRONT);  // 需要剔除的面
  //  glFrontFace(GL_CW);    // 需要剔除面的方向

  const auto program = Program(VertexShader("../shaders/vertex_shaders/depth_testing.vert"),
                               FragmentShader("../shaders/fragment_shaders/depth_testing.frag"));
  program.SetUniform("texture1", 0);
  const auto screen_program =
      Program(VertexShader("../shaders/vertex_shaders/framebuffer.vert"),
              FragmentShader("../shaders/fragment_shaders/framebuffer.frag"));
  screen_program.SetUniform("screenTexture", 1);

  const auto cube_vertices = std::vector<float>{
      // ----- 位置 -----, --- 纹理坐标 ---
      // Back face
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // Bottom-left
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // top-right
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // top-right
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // bottom-left
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // top-left
      // Front face
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // top-right
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // top-right
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // top-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-left
      // Left face
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-right
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-left
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-left
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-right
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-right
      // Right face
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-left
      // Bottom face
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // top-right
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,   // top-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // bottom-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // bottom-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-right
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // top-right
      // Top face
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // bottom-right
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f    // bottom-left
  };
  const auto plane_vertices = std::vector<float>{
      // ----- 位置 -----, --- 纹理坐标 ---
      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,  //
      -5.0f, -0.5f, 5.0f,  0.0f, 0.0f,  //

      5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //
      5.0f,  -0.5f, -5.0f, 2.0f, 2.0f,  //
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f   //
  };
  const auto quad_vertices = std::vector<float>{
      // -- 位置 --, -- 纹理坐标 --
      -1.0f, 1.0f,  0.0f, 1.0f,  //
      -1.0f, -1.0f, 0.0f, 0.0f,  //
      1.0f,  -1.0f, 1.0f, 0.0f,  //

      -1.0f, 1.0f,  0.0f, 1.0f,  //
      1.0f,  -1.0f, 1.0f, 0.0f,  //
      1.0f,  1.0f,  1.0f, 1.0f   //
  };

  // 帧缓冲
  const auto framebuffer = Framebuffer();
  // 生成纹理，作为颜色附件（Retina 屏幕，长宽需 x2）
  const auto texture_color_buffer = Texture(2 * SCR_WIDTH, 2 * SCR_HEIGHT);
  texture_color_buffer.SetFiltering(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture_color_buffer.SetFiltering(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  framebuffer.Attach(texture_color_buffer);
  // 生成渲染缓冲对象，作为深度（和模板）附件（Retina 屏幕，长宽需 x2）
  const auto render_buffer = Renderbuffer(2 * SCR_WIDTH, 2 * SCR_HEIGHT);
  framebuffer.Attach(render_buffer);
  // 检查帧缓冲是否完整
  if (!framebuffer.IsComplete()) {
    std::cerr << "Framebuffer is not complete!" << std::endl;
  }

  // 立方体
  const auto cube_vao = VertexArray();
  const auto cube_vbo = ArrayBuffer(cube_vertices);
  cube_vao.Bind(cube_vbo);
  cube_vao.SetAttribute(0, 3, 5 * sizeof(float), 0);
  cube_vao.SetAttribute(1, 2, 5 * sizeof(float), 3 * sizeof(float));

  // 平面
  const auto plane_vao = VertexArray();
  const auto plane_vbo = ArrayBuffer(plane_vertices);
  plane_vao.Bind(plane_vbo);
  plane_vao.SetAttribute(0, 3, 5 * sizeof(float), 0);
  plane_vao.SetAttribute(1, 2, 5 * sizeof(float), 3 * sizeof(float));

  // 屏幕矩形
  const auto quad_vao = VertexArray();
  const auto quad_vbo = ArrayBuffer(quad_vertices);
  quad_vao.Bind(quad_vbo);
  quad_vao.SetAttribute(0, 2, 4 * sizeof(float), 0);
  quad_vao.SetAttribute(1, 2, 4 * sizeof(float), 2 * sizeof(float));

  const auto cube_texture = Texture("../assets/textures/container.jpg");
  const auto floor_texture = Texture("../assets/textures/metal.png");
  floor_texture.SetWrap(GL_TEXTURE_WRAP_S, GL_REPEAT);
  floor_texture.SetWrap(GL_TEXTURE_WRAP_T, GL_REPEAT);

  // 相机
  auto camera = PerspectiveCamera(45.0f, static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
  camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
  auto control = Control(camera, window);

  // 渲染循环 (render loop)
  while (!glfwWindowShouldClose(window)) {
    // 输入
    HandleKeyboardInput(window);

    framebuffer.Bind();
    glEnable(GL_DEPTH_TEST);

    // 清除缓冲
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    control.Update();

    program.Use();
    program.SetUniform("view", camera.ViewMatrix());
    program.SetUniform("projection", camera.ProjectionMatrix());

    auto model_matrix = glm::mat4(1.0f);

    // 绘制立方体
    cube_vao.Bind();
    cube_texture.BindToUnit(0);
    model_matrix = glm::mat4(1.0f);
    // Y 轴加 0.001 偏移，避免 Z-fighting 问题
    program.SetUniform("model", glm::translate(model_matrix, glm::vec3(-1.0f, 0.001f, -1.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model_matrix = glm::mat4(1.0f);
    program.SetUniform("model", glm::translate(model_matrix, glm::vec3(2.0f, 0.001f, 0.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 绘制平面
    plane_vao.Bind();
    floor_texture.BindToUnit(0);
    program.SetUniform("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 使用默认帧缓冲
    framebuffer.Unbind();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制帧缓冲
    screen_program.Use();
    quad_vao.Bind();
    texture_color_buffer.BindToUnit(1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 交换颜色缓冲
    glfwSwapBuffers(window);

    // 检查触发事件（键盘输入、鼠标移动等）
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
