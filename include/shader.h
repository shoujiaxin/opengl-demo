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
  unsigned int id_;

  // 编译着色器
  void Compile(const std::string& source) const;

  // 读取着色器 GLSL 文件
  static std::string LoadSourceFrom(const std::string& path);

 private:
  // 删除着色器
  void Delete() const;
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
