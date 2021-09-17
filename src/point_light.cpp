//
// Created by Jiaxin Shou on 2021/9/10.
//

#include "light/point_light.h"

PointLight::PointLight(const struct Color &color, float intensity)
    : PositionalLight(color, intensity) {}

PointLight::PointLight(const struct Color &color, float intensity, const glm::vec3 &position)
    : PositionalLight(color, intensity, position) {}

const PointLight::Coefficient &PointLight::Attenuation() const { return attenuation_coefficient_; }

void PointLight::SetAttenuation(const Coefficient &value) { attenuation_coefficient_ = value; }
