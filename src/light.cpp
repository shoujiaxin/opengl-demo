//
// Created by martinshou on 2021/9/6.
//

#include "light.h"

const auto Color::kWhite = Color(1.0f, 1.0f, 1.0f);

glm::vec3 Light::Color() const {
  return intensity_ * glm::vec3(color_.red_, color_.green_, color_.blue_);
}

void Light::SetColor(const glm::vec3 &value) { color_ = ::Color(value.x, value.y, value.z); }

void Light::SetColor(const struct Color &value) { color_ = value; }

void Light::SetColor(float red, float green, float blue) { color_ = ::Color(red, green, blue); }

void Light::SetIntensity(float value) { intensity_ = value; }

Light::Light(const struct Color &color, float intensity) : color_(color), intensity_(intensity) {}

PointLight::PointLight(const struct Color &color, float intensity) : Light(color, intensity) {}

const glm::vec3 &PointLight::Position() const { return position_; }

void PointLight::SetPosition(const glm::vec3 &value) { position_ = value; }

DirectionalLight::DirectionalLight(const struct Color &color, float intensity)
    : Light(color, intensity) {}

const glm::vec3 &DirectionalLight::Direction() const { return direction_; }

void DirectionalLight::SetDirection(const glm::vec3 &value) { direction_ = value; }

AmbientLight::AmbientLight(const struct Color &color, float intensity) : Light(color, intensity) {}
