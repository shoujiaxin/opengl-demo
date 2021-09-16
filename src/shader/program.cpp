//
// Created by Jiaxin Shou on 2021/7/21.
//

#include "program.h"

#include <iostream>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

Program::Program() : id_(glCreateProgram()) {}

Program::Program(const Shader &vertex_shader, const Shader &fragment_shader) : Program() {
  AttachShader(vertex_shader);
  AttachShader(fragment_shader);
  Link();
}

Program::~Program() { glDeleteProgram(id_); }

void Program::BindUniformBlock(const std::string &name, int binding_point) const {
  glUniformBlockBinding(id_, glGetUniformBlockIndex(id_, name.c_str()), binding_point);
}

void Program::Use() const { glUseProgram(id_); }

void Program::AttachShader(const Shader &shader) const { glAttachShader(id_, shader.Id()); }

void Program::Link() const {
  glLinkProgram(id_);

  int success;
  char info_log[512];
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id_, 512, nullptr, info_log);
    std::cerr << "Failed to link program: " << info_log << std::endl;
  }
}

void Program::SetUniform(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void Program::SetUniform(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Program::SetUniform(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Program::SetUniform(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}
