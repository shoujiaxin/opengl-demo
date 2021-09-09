//
// Created by martinshou on 2021/7/21.
//

#pragma once

#include <string>

class Shader {
 public:
  ~Shader();

  // 着色器 ID
  unsigned int Id() const;

 protected:
  // 编译着色器
  void Compile(const std::string& source) const;

  // 读取着色器 GLSL 文件
  static std::string LoadSourceFrom(const std::string& path);

  unsigned int id_ = 0;
};

class VertexShader : public Shader {
 public:
  // 顶点着色器
  explicit VertexShader(const std::string& path);
};

class FragmentShader : public Shader {
 public:
  // 片段着色器
  explicit FragmentShader(const std::string& path);
};
