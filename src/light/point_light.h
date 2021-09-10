//
// Created by Jiaxin Shou on 2021/9/10.
//

#pragma once

#include "positional_light.h"

class PointLight : public PositionalLight {
 public:
  struct Coefficient {
    // 常数项
    float constant_ = 0.0f;

    // 一次项
    float linear_ = 0.0f;

    // 二次项
    float quadratic_ = 0.0f;
  };

  PointLight(const struct Color& color, float intensity);

  PointLight(const struct Color& color, float intensity, const glm::vec3& position);

  [[nodiscard]] const Coefficient& Attenuation() const;

  void SetAttenuation(const Coefficient& value);

 protected:
  // 衰减系数
  Coefficient attenuation_coefficient_ = {1.0f};
};
