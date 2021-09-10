//
// Created by Jiaxin Shou on 2021/9/6.
//

#include "light.h"

const auto Color::kBlack = Color(0.0f, 0.0f, 0.0f);
const auto Color::kBlue = Color(0.0f, 0.0f, 1.0f);
const auto Color::kGreen = Color(0.0f, 1.0f, 0.0f);
const auto Color::kRed = Color(1.0f, 0.0f, 0.0f);
const auto Color::kWhite = Color(1.0f, 1.0f, 1.0f);

Light::Light(const struct Color &color, float intensity) : color_(color), intensity_(intensity) {}

glm::vec3 Light::Color() const { return {color_.red_, color_.green_, color_.blue_}; }

float Light::Intensity() const { return intensity_; }

void Light::SetColor(const glm::vec3 &value) { color_ = ::Color(value.x, value.y, value.z); }

void Light::SetColor(const struct Color &value) { color_ = value; }

void Light::SetColor(float red, float green, float blue) { color_ = ::Color(red, green, blue); }

void Light::SetIntensity(float value) { intensity_ = value; }
