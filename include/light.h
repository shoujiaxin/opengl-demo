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
  Light(const struct Color& color, float intensity);

  glm::vec3 Color() const;

  void SetColor(const glm::vec3& value);

  void SetColor(const struct Color& value);

  void SetColor(float red, float green, float blue);

  void SetIntensity(float value);

 protected:
  struct Color color_ = Color::kWhite;

  float intensity_ = 1.0f;
};

class DirectionalLight : public Light {
 public:
  DirectionalLight(const struct Color& color, float intensity);

  const glm::vec3& Direction() const;

  void SetDirection(const glm::vec3& value);

 private:
  glm::vec3 direction_ = glm::vec3(0.0f, 0.0f, 0.0f);
};

class PointLight : public Light {
 public:
  PointLight(const struct Color& color, float intensity);

  const glm::vec3& Position() const;

  void SetPosition(const glm::vec3& value);

 private:
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
};

class Spotlight : public Light {
 public:
  Spotlight(const struct Color& color, float intensity);

  const glm::vec3& Direction() const;

  float InnerCutOff() const;

  float OuterCutOff() const;

  const glm::vec3& Position() const;

  void SetCutOff(float inner, float outer);

  void SetDirection(const glm::vec3& value);

  void SetInnerCutOff(float value);

  void SetOuterCutOff(float value);

  void SetPosition(const glm::vec3& value);

 private:
  glm::vec3 direction_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 内切光角的余弦值
  float inner_cut_off_ = 0.0f;

  // 外切光角的余弦值
  float outer_cut_off_ = 0.0f;

  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
};

using AmbientLight = Light;
