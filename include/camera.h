//
// Created by martinshou on 2021/9/2.
//

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class PerspectiveCamera {
 public:
  PerspectiveCamera(float fov = 50.0f, float aspect = 1.0f, float near = 0.1f, float far = 2000.0f);

  float Fov() const;

  void LookAt(const glm::vec3& target);

  const glm::vec3& Position() const;

  auto ProjectionMatrix() const -> decltype(glm::value_ptr(glm::mat4()));

  void SetAspect(float aspect);

  void SetFar(float far);

  void SetFov(float fov);

  void SetNear(float near);

  void SetPosition(const glm::vec3& position);

  void UpdateMatrices();

  auto ViewMatrix() const -> decltype(glm::value_ptr(glm::mat4()));

 private:
  // 宽高比
  float aspect_ = 0.0f;

  // 远平面
  float far_ = 0.0f;

  // 视野
  float fov_ = 0.0f;

  // 近平面
  float near_ = 0.0f;

  // 相机位置
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 投影矩阵
  glm::mat4 projection_matrix_ = glm::mat4(1.0f);

  // 目标位置
  glm::vec3 target_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 观察矩阵
  glm::mat4 view_matrix_ = glm::mat4(1.0f);

  // 更新投影矩阵
  void UpdateProjectionMatrix();

  // 更新观察矩阵
  void UpdateViewMatrix();
};
