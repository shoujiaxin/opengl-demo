//
// Created by Jiaxin Shou on 2021/9/10.
//

#pragma once

#include "point_light.h"

class Spotlight : public PointLight {
 public:
  struct CutOff {
    // 内切光角余弦值
    float inner_ = 0.0f;

    // 外切光角余弦值
    float outer_ = 0.0f;
  };

  Spotlight(const struct Color& color, float intensity);

  Spotlight(const struct Color& color, float intensity, const glm::vec3& position,
            const glm::vec3& target);

  [[nodiscard]] const CutOff& CutOff() const;

  [[nodiscard]] glm::vec3 Direction() const;

  void SetCutOff(const struct CutOff& value);

  void SetTarget(const glm::vec3& value);

  [[nodiscard]] const glm::vec3& Target() const;

 private:
  struct CutOff cut_off_ = CutOff();

  glm::vec3 target_ = glm::vec3(0.0f, 0.0f, 0.0f);
};
