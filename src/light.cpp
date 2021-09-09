//
// Created by martinshou on 2021/9/6.
//

#include "light.h"

const auto Color::kWhite = Color(1.0f, 1.0f, 1.0f);

Light::Light(const struct Color &color, float intensity) : color_(color), intensity_(intensity) {}

glm::vec3 Light::Color() const {
  return intensity_ * glm::vec3(color_.red_, color_.green_, color_.blue_);
}

void Light::SetColor(const glm::vec3 &value) { color_ = ::Color(value.x, value.y, value.z); }

void Light::SetColor(const struct Color &value) { color_ = value; }

void Light::SetColor(float red, float green, float blue) { color_ = ::Color(red, green, blue); }

void Light::SetIntensity(float value) { intensity_ = value; }

DirectionalLight::DirectionalLight(const struct Color &color, float intensity)
    : Light(color, intensity) {}

const glm::vec3 &DirectionalLight::Direction() const { return direction_; }

void DirectionalLight::SetDirection(const glm::vec3 &value) { direction_ = value; }

PointLight::PointLight(const struct Color &color, float intensity) : Light(color, intensity) {}

const glm::vec3 &PointLight::Position() const { return position_; }

void PointLight::SetPosition(const glm::vec3 &value) { position_ = value; }

Spotlight::Spotlight(const struct Color &color, float intensity) : Light(color, intensity) {}

const glm::vec3 &Spotlight::Direction() const { return direction_; }

float Spotlight::InnerCutOff() const { return inner_cut_off_; }

float Spotlight::OuterCutOff() const { return outer_cut_off_; }

const glm::vec3 &Spotlight::Position() const { return position_; }

void Spotlight::SetCutOff(float inner, float outer) {
  inner_cut_off_ = inner;
  outer_cut_off_ = outer;
}

void Spotlight::SetDirection(const glm::vec3 &value) { direction_ = value; }

void Spotlight::SetInnerCutOff(float value) { inner_cut_off_ = value; }

void Spotlight::SetOuterCutOff(float value) { outer_cut_off_ = value; }

void Spotlight::SetPosition(const glm::vec3 &value) { position_ = value; }
