//
// Created by Jiaxin Shou on 2021/9/14.
//

#include "vertex_shader.h"

#include "glad/glad.h"

VertexShader::VertexShader(const std::string &path) {
  id_ = glCreateShader(GL_VERTEX_SHADER);
  Compile(LoadSourceFrom(path));
}
