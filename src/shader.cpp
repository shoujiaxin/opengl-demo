//
// Created by martinshou on 2021/7/21.
//

#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/glad.h"

Shader::~Shader() { Delete(); }

unsigned int Shader::Id() const { return id_; }

void Shader::Compile(const std::string &source) const {
  const auto source_str = source.c_str();
  glShaderSource(id_, 1, &source_str, nullptr);
  glCompileShader(id_);

  int success;
  char info_log[512];
  glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id_, 512, nullptr, info_log);
    std::cerr << "Failed to compile shader: " << info_log << std::endl;
  }
}

std::string Shader::LoadSourceFrom(const std::string &path) {
  auto file = std::ifstream(path);
  std::stringstream stream;
  stream << file.rdbuf();
  return stream.str();
}

void Shader::Delete() const { glDeleteShader(id_); }

VertexShader::VertexShader(const std::string &path) {
  id_ = glCreateShader(GL_VERTEX_SHADER);
  Compile(LoadSourceFrom(path));
}

FragmentShader::FragmentShader(const std::string &path) {
  id_ = glCreateShader(GL_FRAGMENT_SHADER);
  Compile(LoadSourceFrom(path));
}
