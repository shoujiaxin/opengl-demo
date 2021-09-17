//
// Created by Jiaxin Shou on 2021/9/6.
//

#include "light/light.h"

Light::Light(const struct Color &color, float intensity) : color_(color), intensity_(intensity) {}

glm::vec3 Light::Color() const { return {color_.red_, color_.green_, color_.blue_}; }

float Light::Intensity() const { return intensity_; }

void Light::SetColor(const glm::vec3 &value) { color_ = ::Color(value.x, value.y, value.z); }

void Light::SetColor(const struct Color &value) { color_ = value; }

void Light::SetColor(float red, float green, float blue) { color_ = ::Color(red, green, blue); }

void Light::SetIntensity(float value) { intensity_ = value; }
