//
// Created by Jiaxin Shou on 2021/8/22.
//

#include "texture.h"

#include <iostream>

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &path, enum Type type) : type_(type) {
  auto channels = 0;
  stbi_set_flip_vertically_on_load(true);
  const auto data = stbi_load(path.c_str(), &width_, &height_, &channels, 0);
  if (data == nullptr) {
    std::cerr << "Failed to load texture: " << path << std::endl;
    return;
  }

  // 创建纹理
  glGenTextures(1, &id_);
  // 绑定纹理
  Bind();
  // 生成纹理
  auto format = GL_RGB;
  switch (channels) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      break;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
  // 生成多级渐远纹理
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

Texture::Texture(int width, int height, enum Format format)
    : format_(format), height_(height), width_(width) {
  glGenTextures(1, &id_);
  Bind();

  switch (format) {
    case Format::kDefault:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      break;
    case Format::kDepthComponent:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
                   GL_UNSIGNED_BYTE, nullptr);
      break;
    case Format::kDepthStencil:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL,
                   GL_UNSIGNED_INT_24_8, NULL);
      break;
    case Format::kStencilIndex:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX,
                   GL_UNSIGNED_BYTE, nullptr);
      break;
    default:
      assert(false);
  }
}

Texture::~Texture() { glDeleteTextures(1, &id_); }

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, id_); }

enum Texture::Format Texture::Format() const { return format_; }

unsigned int Texture::Id() const { return id_; }

void Texture::SetFiltering(int operation, int method) const {
  Bind();
  glTexParameteri(GL_TEXTURE_2D, operation, method);
}

void Texture::SetType(enum Type value) { type_ = value; }

void Texture::SetWrap(int axis, int mode) const {
  Bind();
  glTexParameteri(GL_TEXTURE_2D, axis, mode);
}

enum Texture::Type Texture::Type() const { return type_; }
