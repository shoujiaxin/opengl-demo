//
// Created by Jiaxin Shou on 2021/7/21.
//

#pragma once

#include <string>

#include "glad/glad.h"
#include "interface/identifiable.h"

class Shader : public Identifiable<unsigned int> {
 public:
  Shader(GLenum type, const std::string& path);

  virtual ~Shader();

 protected:
  // 编译着色器
  void Compile(const std::string& source) const;

  // 读取着色器 GLSL 文件
  static std::string LoadSourceFrom(const std::string& path);
};

class FragmentShader final : public Shader {
 public:
  explicit FragmentShader(const std::string& path) : Shader(GL_FRAGMENT_SHADER, path){};
};

class VertexShader final : public Shader {
 public:
  explicit VertexShader(const std::string& path) : Shader(GL_VERTEX_SHADER, path) {}
};
