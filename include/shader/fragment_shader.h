//
// Created by Jiaxin Shou on 2021/10/8.
//

#pragma once

#include "shader.h"

class FragmentShader final : public Shader {
 public:
  explicit FragmentShader(const std::string& path) : Shader(GL_FRAGMENT_SHADER, path) {}
};
