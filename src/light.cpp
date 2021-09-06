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

void Light::SetIntensity(float value) { intensity_ = value; }

Light::Light(const struct Color &color, float intensity) : color_(color), intensity_(intensity) {}

DirectionalLight::DirectionalLight(const struct Color &color, float intensity)
    : Light(color, intensity) {}

const glm::vec3 &DirectionalLight::Position() const { return position_; }

void DirectionalLight::SetPosition(const glm::vec3 &value) { position_ = value; }

AmbientLight::AmbientLight(const struct Color &color, float intensity) : Light(color, intensity) {}

DiffuseLight::DiffuseLight(const struct Color &color, float intensity)
    : DirectionalLight(color, intensity) {}

SpecularLight::SpecularLight(const struct Color &color, float intensity)
    : DirectionalLight(color, intensity) {}