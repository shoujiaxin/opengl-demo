//
// Created by Jiaxin Shou on 2021/10/8.
//

#pragma once

#include "shader.h"

class VertexShader final : public Shader {
 public:
  explicit VertexShader(const std::string& path) : Shader(GL_VERTEX_SHADER, path) {}
};
