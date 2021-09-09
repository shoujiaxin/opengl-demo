//
// Created by martinshou on 2021/9/6.
//

#pragma once

#include "glm/glm.hpp"

struct Color {
  Color(float r, float g, float b) : red_(r), green_(g), blue_(b) {}

  float red_ = 0.0f;
  float green_ = 0.0f;
  float blue_ = 0.0f;

  static const Color kWhite;
};

class Light {
 public:
  glm::vec3 Color() const;

  void SetColor(const glm::vec3& value);

  void SetColor(const struct Color& value);

  void SetIntensity(float value);

 protected:
  struct Color color_ = Color::kWhite;

  float intensity_ = 1.0f;

  Light(const struct Color& color, float intensity);
};

class PointLight : public Light {
 public:
  PointLight(const struct Color& color, float intensity);

  const glm::vec3& Position() const;

  void SetPosition(const glm::vec3& value);

 private:
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
};

class AmbientLight : public Light {
 public:
  AmbientLight(const struct Color& color = Color::kWhite, float intensity = 1.0f);
};
