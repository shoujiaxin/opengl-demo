//
// Created by Jiaxin Shou on 2021/9/14.
//

#include "fragment_shader.h"

#include "glad/glad.h"

FragmentShader::FragmentShader(const std::string &path) {
  id_ = glCreateShader(GL_FRAGMENT_SHADER);
  Compile(LoadSourceFrom(path));
}
