//
// Created by martinshou on 2021/7/21.
//

#ifndef LEARN_OPENGL_SHADER_H
#define LEARN_OPENGL_SHADER_H

#include <string>

#include "glad/glad.h"

class Program {
 public:
  unsigned int id;

  Program(const std::string& vertex_path, const std::string& fragment_path);

  void Delete() const;

  void Set(const std::string& name, bool value) const;

  void Set(const std::string& name, int value) const;

  void Set(const std::string& name, float value) const;

  void Use() const;

 private:
  static std::string GetSourceFrom(const std::string& path);
};

// Inline functions

inline void Program::Delete() const { glDeleteProgram(id); }

inline void Program::Set(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

inline void Program::Set(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

inline void Program::Set(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

inline void Program::Use() const { glUseProgram(id); }

#endif  // LEARN_OPENGL_SHADER_H
