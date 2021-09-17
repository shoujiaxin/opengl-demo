//
// Created by Jiaxin Shou on 2021/9/2.
//

#pragma once

#include "glm/glm.hpp"

class Camera {
 public:
  [[nodiscard]] virtual const glm::vec3& Position() const;

  virtual void SetPosition(const glm::vec3& value);

  virtual void UpdateMatrices() = 0;

 protected:
  // 相机位置
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
};
