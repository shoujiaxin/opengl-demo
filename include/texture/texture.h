//
// Created by Jiaxin Shou on 2021/8/22.
//

#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"
#include "interface/bindable.h"
#include "interface/identifiable.h"

class Texture final : public Bindable, public Identifiable<unsigned int> {
 public:
  enum class Type { kDefault, kCubeMapping, kDiffuseMapping, kReflectionMapping, kSpecularMapping };

  enum class Format { kDefault, kDepthComponent, kDepthStencil, kStencilIndex };

  explicit Texture(const std::string& path, Type type = Type::kDefault);

  // 创建指定尺寸的空纹理
  Texture(int width, int height, Format format = Format::kDefault);

  // 创建立方体贴图
  explicit Texture(const std::vector<std::string>& paths);

  ~Texture();

  // 绑定纹理
  void Bind() const override;

  // 绑定到指定纹理单元
  void BindToUnit(int index) const;

  [[nodiscard]] Format Format() const;

  // 设置过滤方式
  void SetFiltering(int operation, int method) const;

  void SetType(Type value);

  // 设置环绕方式
  void SetWrap(int axis, int mode) const;

  [[nodiscard]] Type Type() const;

  void Unbind() const override;

 private:
  explicit Texture(enum Type type);

  [[nodiscard]] GLenum Target() const;

  enum Format format_ = Format::kDefault;

  int height_ = 0;

  enum Type type_ = Type::kDefault;

  int width_ = 0;
};
