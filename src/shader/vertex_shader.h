//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

#include "shader.h"

class VertexShader final : public Shader {
 public:
  explicit VertexShader(const std::string& path);
};
