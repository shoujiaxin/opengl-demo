//
// Created by Jiaxin Shou on 2021/7/21.
//

#include "shader/shader.h"

#include <fstream>
#include <sstream>

#include "spdlog/spdlog.h"

Shader::Shader(GLenum type, const std::string &path)
    : Identifiable<unsigned int>(glCreateShader(type)) {
  Compile(LoadSourceFrom(path));
}

Shader::~Shader() { glDeleteShader(id_); }

void Shader::Compile(const std::string &source) const {
  const auto source_str = source.c_str();
  glShaderSource(id_, 1, &source_str, nullptr);
  glCompileShader(id_);

  int success;
  char info_log[512];
  glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id_, 512, nullptr, info_log);
    spdlog::error("failed to compile shader: {0}", info_log);
  }
}

std::string Shader::LoadSourceFrom(const std::string &path) {
  auto file = std::ifstream(path);
  if (file.fail()) {
    spdlog::error("failed to load shader source: {0}", path);
    return "";
  }
  std::stringstream stream;
  stream << file.rdbuf();
  spdlog::info("shader source loaded: {0}", path);
  return stream.str();
}
