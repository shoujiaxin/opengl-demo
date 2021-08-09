//
// Created by martinshou on 2021/7/21.
//

#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::~Shader() { Delete(); }

void Shader::Delete() const { glDeleteShader(id_); }

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
  return std::move(stream.str());
}

VertexShader::VertexShader(const std::string &path) {
  id_ = glCreateShader(GL_VERTEX_SHADER);
  const auto source = LoadSourceFrom(path);
  Compile(source);
}

FragmentShader::FragmentShader(const std::string &path) {
  id_ = glCreateShader(GL_FRAGMENT_SHADER);
  const auto source = LoadSourceFrom(path);
  Compile(source);
}
