//
// Created by martinshou on 2021/9/3.
//

#include "controls.h"

#include <algorithm>

#define CAMERA_UP glm::vec3(0.0f, 1.0f, 0.0f)

Controls::Controls(PerspectiveCamera &camera, GLFWwindow *window)
    : camera_(camera), window_(window) {
  glfwSetWindowUserPointer(window, this);

  glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
    auto width = 0;
    auto height = 0;
    glfwGetWindowSize(window, &width, &height);
    auto controls = static_cast<Controls *>(glfwGetWindowUserPointer(window));
    controls->OnMouseMove(x / width, y / height);
  });

  glfwSetScrollCallback(window, [](GLFWwindow *window, double x, double y) {
    auto controls = static_cast<Controls *>(glfwGetWindowUserPointer(window));
    controls->OnScroll(x, y);
  });
}

void Controls::Update() {
  // 计算两帧之间的时间差
  const auto current_time = glfwGetTime();
  const auto delta_time = current_time - last_frame_time_;
  last_frame_time_ = current_time;

  OnKeyboardPress(delta_time);
  camera_.LookAt(camera_.Position() + camera_front_);
}

void Controls::OnKeyboardPress(float dt) {
  const auto camera_move_speed = 2.5f * dt;

  auto camera_position = camera_.Position();
  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
    camera_position += camera_move_speed * camera_front_;
  }
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
    camera_position -= camera_move_speed * camera_front_;
  }
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
    camera_position -= camera_move_speed * glm::normalize(glm::cross(camera_front_, CAMERA_UP));
  }
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
    camera_position += camera_move_speed * glm::normalize(glm::cross(camera_front_, CAMERA_UP));
  }
  camera_.SetPosition(camera_position);
}

void Controls::OnMouseMove(double x, double y) {
  if (first_cursor_) {
    first_cursor_ = false;
    last_cursor_position_.x = x;
    last_cursor_position_.y = y;
  }

  constexpr auto sensitivity = 100.0f;
  const auto offset_x = sensitivity * (x - last_cursor_position_.x);
  const auto offset_y = sensitivity * (last_cursor_position_.y - y);
  camera_yaw_ += offset_x;
  camera_pitch_ += offset_y;

  auto target = glm::vec3(0.0f, 0.0f, 0.0f);
  target.x = cos(glm::radians(camera_yaw_)) * cos(glm::radians(camera_pitch_));
  target.y = sin(glm::radians(camera_pitch_));
  target.z = sin(glm::radians(camera_yaw_)) * cos(glm::radians(camera_pitch_));
  camera_front_ = glm::normalize(target);

  last_cursor_position_.x = x;
  last_cursor_position_.y = y;
}

void Controls::OnScroll(double x, double y) {
  auto fov = camera_.Fov();
  fov -= y;
  fov = std::min(fov, 50.0f);
  fov = std::max(fov, 0.1f);
  camera_.SetFov(fov);
}
