//
// Created by Jiaxin Shou on 2021/9/2.
//

#include "camera.h"

const glm::vec3& Camera::Position() const { return position_; }

void Camera::SetPosition(const glm::vec3& value) { position_ = value; }
