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
  const auto guard = BindGuard(*this);
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
  const auto guard = BindGuard(*this);

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
                   GL_UNSIGNED_INT_24_8, nullptr);
      break;
    case Format::kStencilIndex:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX,
                   GL_UNSIGNED_BYTE, nullptr);
      break;
    default:
      assert(false);
  }
}

Texture::Texture(const std::vector<std::string> &paths) {
  glGenTextures(1, &id_);
  const auto guard = BindGuard(*this);

  auto channels = 0;
  stbi_set_flip_vertically_on_load(false);
  for (auto i = 0; i < paths.size(); ++i) {
    const auto &path = paths[i];
    const auto data = stbi_load(path.c_str(), &width_, &height_, &channels, 0);
    if (data == nullptr) {
      std::cerr << "Failed to load texture: " << path << std::endl;
      return;
    }

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
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width_, height_, 0, format,
                 GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture::~Texture() { glDeleteTextures(1, &id_); }

void Texture::Bind() const {
  if (type_ == Type::kCubeMapping) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
  } else {
    glBindTexture(GL_TEXTURE_2D, id_);
  }
}

enum Texture::Format Texture::Format() const { return format_; }

unsigned int Texture::Id() const { return id_; }

void Texture::SetFiltering(int operation, int method) const {
  const auto guard = BindGuard(*this);
  if (type_ == Type::kCubeMapping) {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, operation, method);
  } else {
    glTexParameteri(GL_TEXTURE_2D, operation, method);
  }
}

void Texture::SetType(enum Type value) { type_ = value; }

void Texture::SetWrap(int axis, int mode) const {
  const auto guard = BindGuard(*this);
  if (type_ == Type::kCubeMapping) {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, axis, mode);
  } else {
    glTexParameteri(GL_TEXTURE_2D, axis, mode);
  }
}

enum Texture::Type Texture::Type() const { return type_; }

void Texture::Unbind() const {
  if (type_ == Type::kCubeMapping) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
