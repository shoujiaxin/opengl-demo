//
// Created by martinshou on 2021/9/6.
//

#include "light.h"

// Color
const auto Color::kBlack = Color(0.0f, 0.0f, 0.0f);
const auto Color::kBlue = Color(0.0f, 0.0f, 1.0f);
const auto Color::kGreen = Color(0.0f, 1.0f, 0.0f);
const auto Color::kRed = Color(1.0f, 0.0f, 0.0f);
const auto Color::kWhite = Color(1.0f, 1.0f, 1.0f);

// Light
Light::Light(const struct Color &color, float intensity) : color_(color), intensity_(intensity) {}

glm::vec3 Light::Color() const { return glm::vec3(color_.red_, color_.green_, color_.blue_); }

float Light::Intensity() const { return intensity_; }

void Light::SetColor(const glm::vec3 &value) { color_ = ::Color(value.x, value.y, value.z); }

void Light::SetColor(const struct Color &value) { color_ = value; }

void Light::SetColor(float red, float green, float blue) { color_ = ::Color(red, green, blue); }

void Light::SetIntensity(float value) { intensity_ = value; }

// DirectionalLight
DirectionalLight::DirectionalLight(const struct Color &color, float intensity)
    : Light(color, intensity) {}

DirectionalLight::DirectionalLight(const struct Color &color, float intensity,
                                   const glm::vec3 &direction)
    : Light(color, intensity), direction_(direction) {}

const glm::vec3 &DirectionalLight::Direction() const { return direction_; }

void DirectionalLight::SetDirection(const glm::vec3 &value) { direction_ = value; }

// PositionalLight
const glm::vec3 &PositionalLight::Position() const { return position_; }

void PositionalLight::SetPosition(const glm::vec3 &value) { position_ = value; }

PositionalLight::PositionalLight(const struct Color &color, float intensity)
    : Light(color, intensity) {}

PositionalLight::PositionalLight(const struct Color &color, float intensity,
                                 const glm::vec3 &position)
    : Light(color, intensity), position_(position) {}

// PointLight
PointLight::PointLight(const struct Color &color, float intensity)
    : PositionalLight(color, intensity) {}

PointLight::PointLight(const struct Color &color, float intensity, const glm::vec3 &position)
    : PositionalLight(color, intensity, position) {}

const PointLight::Coefficient &PointLight::Attenuation() const { return attenuation_coefficient_; }

void PointLight::SetAttenuation(const Coefficient &value) { attenuation_coefficient_ = value; }

// Spotlight
Spotlight::Spotlight(const struct Color &color, float intensity) : PointLight(color, intensity) {}

Spotlight::Spotlight(const struct Color &color, float intensity, const glm::vec3 &position,
                     const glm::vec3 &target)
    : PointLight(color, intensity, position), target_(target) {}

const struct Spotlight::CutOff &Spotlight::CutOff() const { return cut_off_; }

glm::vec3 Spotlight::Direction() const { return target_ - position_; }

void Spotlight::SetCutOff(const struct CutOff &value) { cut_off_ = value; }

void Spotlight::SetTarget(const glm::vec3 &value) { target_ = value; }

const glm::vec3 &Spotlight::Target() const { return target_; }
