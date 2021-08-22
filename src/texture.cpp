//
// Created by Jiaxin Shou on 2021/8/22.
//

#include "texture.h"

#include <iostream>

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &path) {
  int channels;
  stbi_set_flip_vertically_on_load(true);
  const auto data = stbi_load(path.c_str(), &width_, &height_, &channels, 0);
  if (data == nullptr) {
    std::cerr << "Failed to load texture" << std::endl;
    return;
  }

  // 创建纹理
  glGenTextures(1, &id_);
  // 绑定纹理
  glBindTexture(GL_TEXTURE_2D, id_);
  // 生成纹理
  const auto format = (path.substr(path.size() - 4, 4) == ".png") ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
  // 生成多级渐远纹理
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, id_); }
