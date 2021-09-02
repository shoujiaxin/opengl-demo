//
// Created by martinshou on 2021/9/2.
//

#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(const glm::vec3& position) { SetPosition(position); }

void Camera::LookAt(const glm::vec3& target) {
  target_ = target;
  UpdateViewMatrix();
}

void Camera::SetPosition(const glm::vec3& position) {
  position_ = position;
  UpdateViewMatrix();
}

auto Camera::ViewMatrix() const -> decltype(glm::value_ptr(glm::mat4())) {
  return glm::value_ptr(view_matrix_);
}

void Camera::UpdateViewMatrix() {
  view_matrix_ = glm::lookAt(position_, target_, glm::vec3(0.0f, 1.0f, 0.0f));
}