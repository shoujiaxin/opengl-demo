//
// Created by Jiaxin Shou on 2021/10/8.
//

#pragma once

#include "shader.h"

class GeometryShader final : public Shader {
 public:
  explicit GeometryShader(const std::string& path) : Shader(GL_GEOMETRY_SHADER, path) {}
};
