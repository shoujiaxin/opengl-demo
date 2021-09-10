//
// Created by Jiaxin Shou on 2021/9/10.
//

#pragma once

#include "light.h"

class PositionalLight : public Light {
 public:
  [[nodiscard]] const glm::vec3& Position() const;

  void SetPosition(const glm::vec3& value);

 protected:
  PositionalLight(const struct Color& color, float intensity);

  PositionalLight(const struct Color& color, float intensity, const glm::vec3& position);

  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
};
