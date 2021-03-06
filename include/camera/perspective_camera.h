//
// Created by Jiaxin Shou on 2021/9/10.
//

#pragma once

#include "camera.h"

class PerspectiveCamera final : public Camera {
 public:
  explicit PerspectiveCamera(float fov = 50.0f, float aspect = 1.0f, float near = 0.1f,
                             float far = 2000.0f);

  [[nodiscard]] float Fov() const;

  [[nodiscard]] glm::vec3 Front() const;

  void LookAt(const glm::vec3& target);

  [[nodiscard]] const glm::mat4& ProjectionMatrix() const;

  void SetAspect(float value);

  void SetFar(float value);

  void SetFov(float value);

  void SetNear(float value);

  void SetPosition(const glm::vec3& value) override;

  [[nodiscard]] const glm::vec3& Target() const;

  void UpdateMatrices() override;

  [[nodiscard]] const glm::mat4& ViewMatrix() const;

 private:
  // 更新投影矩阵
  void UpdateProjectionMatrix();

  // 更新观察矩阵
  void UpdateViewMatrix();

  // 宽高比
  float aspect_ = 0.0f;

  // 远平面
  float far_ = 0.0f;

  // 视野
  float fov_ = 0.0f;

  // 近平面
  float near_ = 0.0f;

  // 投影矩阵
  glm::mat4 projection_matrix_ = glm::mat4(1.0f);

  // 目标位置
  glm::vec3 target_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 观察矩阵
  glm::mat4 view_matrix_ = glm::mat4(1.0f);
};
