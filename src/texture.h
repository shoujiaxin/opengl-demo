//
// Created by Jiaxin Shou on 2021/8/22.
//

#pragma once

#include <string>

class Texture {
 public:
  enum class Type { kDefault, kDiffuseMapping, kSpecularMapping };

  enum class Format { kDefault, kDepthComponent, kDepthStencil, kStencilIndex };

  explicit Texture(const std::string& path, Type type = Type::kDefault);

  // 创建指定尺寸的空纹理
  Texture(int width, int height, Format format = Format::kDefault);

  ~Texture();

  // 绑定纹理
  void Bind() const;

  [[nodiscard]] Format Format() const;

  [[nodiscard]] unsigned int Id() const;

  // 设置过滤方式
  void SetFiltering(int operation, int method) const;

  void SetType(Type value);

  // 设置环绕方式
  void SetWrap(int axis, int mode) const;

  [[nodiscard]] Type Type() const;

 private:
  enum Format format_ = Format::kDefault;

  int height_ = 0;

  unsigned int id_ = 0;

  enum Type type_ = Type::kDefault;

  int width_ = 0;
};
