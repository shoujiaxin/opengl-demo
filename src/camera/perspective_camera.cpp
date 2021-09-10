//
// Created by Jiaxin Shou on 2021/9/10.
//

#include "perspective_camera.h"

#include "glm/gtc/matrix_transform.hpp"

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : fov_(fov), aspect_(aspect), near_(near), far_(far) {
  PerspectiveCamera::UpdateMatrices();
}

float PerspectiveCamera::Fov() const { return fov_; }

glm::vec3 PerspectiveCamera::Front() const { return target_ - position_; }

void PerspectiveCamera::LookAt(const glm::vec3& target) {
  target_ = target;
  UpdateViewMatrix();
}

const glm::mat4& PerspectiveCamera::ProjectionMatrix() const { return projection_matrix_; }

void PerspectiveCamera::SetAspect(float value) {
  aspect_ = value;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetFar(float value) {
  far_ = value;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetFov(float value) {
  fov_ = value;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetNear(float value) {
  near_ = value;
  UpdateProjectionMatrix();
}

void PerspectiveCamera::SetPosition(const glm::vec3& value) {
  position_ = value;
  UpdateViewMatrix();
}

const glm::vec3& PerspectiveCamera::Target() const { return target_; }

void PerspectiveCamera::UpdateMatrices() {
  UpdateProjectionMatrix();
  UpdateViewMatrix();
}

const glm::mat4& PerspectiveCamera::ViewMatrix() const { return view_matrix_; }

void PerspectiveCamera::UpdateProjectionMatrix() {
  projection_matrix_ = glm::perspective(glm::radians(fov_), aspect_, near_, far_);
}

void PerspectiveCamera::UpdateViewMatrix() {
  view_matrix_ = glm::lookAt(position_, target_, glm::vec3(0.0f, 1.0f, 0.0f));
}
