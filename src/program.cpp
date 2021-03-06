//
// Created by Jiaxin Shou on 2021/7/21.
//

#include "shader/program.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader/fragment_shader.h"
#include "shader/geometry_shader.h"
#include "shader/vertex_shader.h"
#include "spdlog/spdlog.h"

Program::Program() : Identifiable<unsigned int>(glCreateProgram()) {}

Program::Program(const std::string &vertex_shader, const std::string &fragment_shader) : Program() {
  AttachShader(VertexShader(vertex_shader));
  AttachShader(FragmentShader(fragment_shader));
  Link();
}

Program::Program(const std::string &vertex_shader, const std::string &geometry_shader,
                 const std::string &fragment_shader)
    : Program() {
  AttachShader(VertexShader(vertex_shader));
  AttachShader(GeometryShader(geometry_shader));
  AttachShader(FragmentShader(fragment_shader));
  Link();
}

Program::~Program() { glDeleteProgram(id_); }

void Program::AttachShader(const Shader &shader) const { glAttachShader(id_, shader.Id()); }

void Program::BindUniformBlock(const std::string &name, int binding_point) const {
  Use();
  glUniformBlockBinding(id_, glGetUniformBlockIndex(id_, name.c_str()), binding_point);
}

void Program::Link() const {
  glLinkProgram(id_);

  int success;
  char info_log[512];
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id_, 512, nullptr, info_log);
    spdlog::error("failed to link program (id = {0}): {1}", id_, info_log);
  } else {
    spdlog::info("program (id = {0}) linked", id_);
  }
}

void Program::SetUniform(const std::string &name, bool value) const {
  Use();
  glUniform1i(glGetUniformLocation(id_, name.c_str()), static_cast<int>(value));
}

void Program::SetUniform(const std::string &name, float value) const {
  Use();
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Program::SetUniform(const std::string &name, double value) const {
  Use();
  glUniform1f(glGetUniformLocation(id_, name.c_str()), static_cast<float>(value));
}

void Program::SetUniform(const std::string &name, int value) const {
  Use();
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Program::SetUniform(const std::string &name, const glm::mat4 &value) const {
  Use();
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const glm::vec3 &value) const {
  Use();
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Program::Use() const { glUseProgram(id_); }
