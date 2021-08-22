//
// Created by martinshou on 2021/7/21.
//

#pragma once

#include "shader.h"

class Program {
 public:
  Program();

  Program(const Shader& vertex_shader, const Shader& fragment_shader);

  ~Program();

  void SetUniform(const std::string& name, bool value) const;

  void SetUniform(const std::string& name, int value) const;

  void SetUniform(const std::string& name, float value) const;

  // 使用着色器程序
  void Use() const;

 private:
  unsigned int id_;

  // 添加着色器
  void AttachShader(const Shader& shader) const;

  // 删除着色器程序
  void Delete() const;

  // 连接着色器
  void Link() const;
};
