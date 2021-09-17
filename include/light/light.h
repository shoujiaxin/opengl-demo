//
// Created by Jiaxin Shou on 2021/9/6.
//

#pragma once

#include <algorithm>

#include "glm/glm.hpp"
#include "util/color.h"

class Light {
 public:
  Light(const struct Color& color, float intensity);

  [[nodiscard]] glm::vec3 Color() const;

  [[nodiscard]] float Intensity() const;

  void SetColor(const glm::vec3& value);

  void SetColor(const struct Color& value);

  void SetColor(float red, float green, float blue);

  void SetIntensity(float value);

 protected:
  struct Color color_ = Color::kWhite;

  float intensity_ = 1.0f;
};

using AmbientLight = Light;
