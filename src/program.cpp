//
// Created by martinshou on 2021/7/21.
//

#include "program.h"

#include <fstream>
#include <iostream>
#include <sstream>

Program::Program(const std::string &vertex_path, const std::string &fragment_path) {
  int success;
  char info_log[512];

  // 顶点着色器
  const auto vertex_shader_source = GetSourceFrom(vertex_path);
  const auto v_shader_source = vertex_shader_source.c_str();
  const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &v_shader_source, nullptr);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cerr << "Failed to compile shader: " << info_log << std::endl;
  }

  // 片段着色器
  const auto fragment_shader_source = GetSourceFrom(fragment_path);
  const auto f_shader_source = fragment_shader_source.c_str();
  const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &f_shader_source, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cerr << "Failed to compile shader: " << info_log << std::endl;
  }

  // 着色器程序
  id = glCreateProgram();
  glAttachShader(id, vertex_shader);
  glAttachShader(id, fragment_shader);
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, nullptr, info_log);
    std::cerr << "Link program failed: " << info_log << std::endl;
  }

  // 删除着色器
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

// Private functions

std::string Program::GetSourceFrom(const std::string &path) {
  auto file = std::ifstream(path);
  std::stringstream stream;
  stream << file.rdbuf();
  return std::move(stream.str());
}
