//
// Created by martinshou on 2021/9/2.
//

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera {
 public:
  Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));

  void LookAt(const glm::vec3& target);

  void SetPosition(const glm::vec3& position);

  auto ViewMatrix() const -> decltype(glm::value_ptr(glm::mat4()));

 private:
  // 摄像机位置
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 目标位置
  glm::vec3 target_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 观察矩阵
  glm::mat4 view_matrix_ = glm::mat4();

  // 更新观察矩阵
  void UpdateViewMatrix();
};
