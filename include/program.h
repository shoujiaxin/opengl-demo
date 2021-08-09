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

  void Use() const;

 private:
  void AttachShader(const Shader& shader) const;

  void Delete() const;

  void Link() const;

  void SetUniform(const std::string& name, bool value) const;

  void SetUniform(const std::string& name, int value) const;

  void SetUniform(const std::string& name, float value) const;

  unsigned int id_;
};
