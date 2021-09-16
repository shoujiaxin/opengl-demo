//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

#include "../src/shader/program.h"

class FragmentShader final : public Shader {
 public:
  explicit FragmentShader(const std::string& path) : Shader(GL_FRAGMENT_SHADER, path){};
};

class VertexShader final : public Shader {
 public:
  explicit VertexShader(const std::string& path) : Shader(GL_VERTEX_SHADER, path) {}
};
