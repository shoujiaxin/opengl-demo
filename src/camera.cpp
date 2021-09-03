//
// Created by martinshou on 2021/9/2.
//

#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : fov_(fov), aspect_(aspect), near_(near), far_(far) {
  UpdateProjectionMatrix();
  UpdateViewMatrix();
}

float PerspectiveCamera::Fov() const { return fov_; }

void PerspectiveCamera::LookAt(const glm::vec3& target) {
  target_ = target;
  UpdateViewMatrix();
}

const glm::vec3& PerspectiveCamera::Position() const { return position_; }

auto PerspectiveCamera::ProjectionMatrix() const -> decltype(glm::value_ptr(glm::mat4())) {
  return glm::value_ptr(projection_matrix_);
}

void PerspectiveCamera::SetAspect(float aspect) {
  aspect_ = aspect;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetFar(float far) {
  far_ = far;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetFov(float fov) {
  fov_ = fov;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetNear(float near) {
  near_ = near;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetPosition(const glm::vec3& position) {
  position_ = position;
  UpdateViewMatrix();
}

auto PerspectiveCamera::ViewMatrix() const -> decltype(glm::value_ptr(glm::mat4())) {
  return glm::value_ptr(view_matrix_);
}

void PerspectiveCamera::UpdateProjectionMatrix() {
  projection_matrix_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
}

void PerspectiveCamera::UpdateViewMatrix() {
  view_matrix_ = glm::lookAt(position_, target_, glm::vec3(0.0f, 1.0f, 0.0f));
}
