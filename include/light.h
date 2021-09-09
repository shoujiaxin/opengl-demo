//
// Created by martinshou on 2021/9/6.
//

#pragma once

#include <algorithm>
#include <vector>

#include "glm/glm.hpp"

struct Color {
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

  glm::vec3 Color() const;

  float Intensity() const;

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

  DirectionalLight(const struct Color& color, float intensity, const glm::vec3& direction);

  const glm::vec3& Direction() const;

  void SetDirection(const glm::vec3& value);

 private:
  glm::vec3 direction_ = glm::vec3(0.0f, -1.0f, 0.0f);  // 默认方向竖直向下
};

class PositionalLight : public Light {
 public:
  const glm::vec3& Position() const;

  void SetPosition(const glm::vec3& value);

 protected:
  PositionalLight(const struct Color& color, float intensity);

  PositionalLight(const struct Color& color, float intensity, const glm::vec3& position);

  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
};

class PointLight : public PositionalLight {
 public:
  struct Coefficient {
    float constant = 0.0f;   // 常数项
    float linear = 0.0f;     // 一次项
    float quadratic = 0.0f;  // 二次项
  };

  PointLight(const struct Color& color, float intensity);

  PointLight(const struct Color& color, float intensity, const glm::vec3& position);

  const Coefficient& Attenuation() const;

  void SetAttenuation(const Coefficient& value);

 protected:
  // 衰减系数
  Coefficient attenuation_coefficient_ = {1.0f};
};

class Spotlight : public PointLight {
 public:
  struct CutOff {
    float inner = 0.0f;  // 内切光角余弦值
    float outer = 0.0f;  // 外切光角余弦值
  };

  Spotlight(const struct Color& color, float intensity);

  Spotlight(const struct Color& color, float intensity, const glm::vec3& position,
            const glm::vec3& target);

  const CutOff& CutOff() const;

  glm::vec3 Direction() const;

  void SetCutOff(const struct CutOff& value);

  void SetTarget(const glm::vec3& value);

  const glm::vec3& Target() const;

 private:
  struct CutOff cut_off_ = CutOff();

  glm::vec3 target_ = glm::vec3(0.0f, 0.0f, 0.0f);
};

using AmbientLight = Light;
