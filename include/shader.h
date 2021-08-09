//
// Created by martinshou on 2021/7/21.
//

#pragma once

#include <string>

#include "glad/glad.h"

class Shader {
 public:
  ~Shader();

  void Delete() const;

  unsigned int Id() const;

 protected:
  unsigned int id_;

  void Compile(const std::string& source) const;

  static std::string LoadSourceFrom(const std::string& path);
};

class VertexShader : public Shader {
 public:
  explicit VertexShader(const std::string& path);
};

class FragmentShader : public Shader {
 public:
  explicit FragmentShader(const std::string& path);
};
