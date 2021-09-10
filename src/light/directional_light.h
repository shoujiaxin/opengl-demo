//
// Created by Jiaxin Shou on 2021/9/10.
//

#pragma once

#include "light.h"

class DirectionalLight : public Light {
 public:
  DirectionalLight(const struct Color& color, float intensity);

  DirectionalLight(const struct Color& color, float intensity, const glm::vec3& direction);

  [[nodiscard]] const glm::vec3& Direction() const;

  void SetDirection(const glm::vec3& value);

 private:
  // 方向，默认竖直向下
  glm::vec3 direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
};
