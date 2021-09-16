//
// Created by Jiaxin Shou on 2021/9/6.
//

#pragma once

#include <algorithm>

#include "glm/glm.hpp"

struct Color final {
  static const Color kBlack;
  static const Color kBlue;
  static const Color kGreen;
  static const Color kRed;
  static const Color kWhite;

  Color(float r, float g, float b)
      : red_(std::clamp(r, 0.0f, 1.0f)),
        green_(std::clamp(g, 0.0f, 1.0f)),
        blue_(std::clamp(b, 0.0f, 1.0f)) {}

  float red_ = 0.0f;
  float green_ = 0.0f;
  float blue_ = 0.0f;
};

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
