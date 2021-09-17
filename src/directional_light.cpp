//
// Created by Jiaxin Shou on 2021/9/10.
//

#include "light/directional_light.h"

DirectionalLight::DirectionalLight(const struct Color &color, float intensity)
    : Light(color, intensity) {}

DirectionalLight::DirectionalLight(const struct Color &color, float intensity,
                                   const glm::vec3 &direction)
    : Light(color, intensity), direction_(direction) {}

const glm::vec3 &DirectionalLight::Direction() const { return direction_; }

void DirectionalLight::SetDirection(const glm::vec3 &value) { direction_ = value; }
