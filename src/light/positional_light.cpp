//
// Created by Jiaxin Shou on 2021/9/10.
//

#include "positional_light.h"

const glm::vec3 &PositionalLight::Position() const { return position_; }

void PositionalLight::SetPosition(const glm::vec3 &value) { position_ = value; }

PositionalLight::PositionalLight(const struct Color &color, float intensity)
    : Light(color, intensity) {}

PositionalLight::PositionalLight(const struct Color &color, float intensity,
                                 const glm::vec3 &position)
    : Light(color, intensity), position_(position) {}
