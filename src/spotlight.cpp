//
// Created by Jiaxin Shou on 2021/9/10.
//

#include "light/spotlight.h"

Spotlight::Spotlight(const struct Color &color, float intensity) : PointLight(color, intensity) {}

Spotlight::Spotlight(const struct Color &color, float intensity, const glm::vec3 &position,
                     const glm::vec3 &target)
    : PointLight(color, intensity, position), target_(target) {}

const struct Spotlight::CutOff &Spotlight::CutOff() const { return cut_off_; }

glm::vec3 Spotlight::Direction() const { return target_ - position_; }

void Spotlight::SetCutOff(const struct CutOff &value) { cut_off_ = value; }

void Spotlight::SetTarget(const glm::vec3 &value) { target_ = value; }

const glm::vec3 &Spotlight::Target() const { return target_; }
