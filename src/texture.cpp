//
// Created by Jiaxin Shou on 2021/8/22.
//

#include "texture/texture.h"

#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &path, enum Type type) : Texture(type) {
  auto channels = 0;
  stbi_set_flip_vertically_on_load(true);
  const auto data = stbi_load(path.c_str(), &width_, &height_, &channels, 0);
  if (data == nullptr) {
    spdlog::error("failed to load texture: {0}", path);
    return;
  }
  spdlog::info("texture loaded: {0}", path);

  const auto guard = BindGuard(*this);
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
    : format_(format), height_(height), type_(Type::kDefault), width_(width) {
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

Texture::Texture(const std::vector<std::string> &paths) : Texture(Type::kCubeMapping) {
  const auto guard = BindGuard(*this);

  auto channels = 0;
  stbi_set_flip_vertically_on_load(false);
  for (auto i = 0; i < paths.size(); ++i) {
    const auto &path = paths[i];
    const auto data = stbi_load(path.c_str(), &width_, &height_, &channels, 0);
    if (data == nullptr) {
      spdlog::error("failed to load texture: {0}", path);
      return;
    }
    spdlog::info("texture loaded: {0}", path);

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

void Texture::Bind() const { glBindTexture(Target(), id_); }

void Texture::BindToUnit(int index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  Bind();
  glActiveTexture(GL_TEXTURE0);
}

enum Texture::Format Texture::Format() const { return format_; }

void Texture::SetFiltering(int operation, int method) const {
  const auto guard = BindGuard(*this);
  glTexParameteri(Target(), operation, method);
}

void Texture::SetType(enum Type value) { type_ = value; }

void Texture::SetWrap(int axis, int mode) const {
  const auto guard = BindGuard(*this);
  glTexParameteri(Target(), axis, mode);
}

enum Texture::Type Texture::Type() const { return type_; }

void Texture::Unbind() const { glBindTexture(Target(), 0); }

Texture::Texture(enum Type type) : type_(type) { glGenTextures(1, &id_); }

GLenum Texture::Target() const {
  switch (type_) {
    case Type::kCubeMapping:
      return GL_TEXTURE_CUBE_MAP;
    default:
      return GL_TEXTURE_2D;
  }
}
