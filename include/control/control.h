//
// Created by Jiaxin Shou on 2021/9/3.
//

#pragma once

#include "GLFW/glfw3.h"
#include "camera/perspective_camera.h"

class Control final {
 public:
  Control(PerspectiveCamera& camera, GLFWwindow* window);

  void Update();

 private:
  void OnKeyboardPress(float dt);

  void OnMouseMove(float x, float y);

  void OnScroll(float x, float y);

  PerspectiveCamera& camera_;

  glm::vec3 camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);

  float camera_pitch_ = 0.0f;

  float camera_yaw_ = -90.0f;

  bool first_cursor_ = true;

  glm::vec2 last_cursor_position_ = glm::vec2(0.0f, 0.0f);

  double last_frame_time_ = 0.0;

  GLFWwindow* window_;
};
