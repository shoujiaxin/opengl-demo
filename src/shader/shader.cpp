//
// Created by Jiaxin Shou on 2021/7/21.
//

#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/glad.h"

Shader::~Shader() { glDeleteShader(id_); }

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
  if (file.fail()) {
    std::cerr << "File not found: " << path << std::endl;
    return "";
  }
  std::stringstream stream;
  stream << file.rdbuf();
  return stream.str();
}
