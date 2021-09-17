//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

#include "interface/bindable.h"
#include "interface/identifiable.h"
#include "renderbuffer.h"
#include "texture/texture.h"

class Framebuffer final : public Bindable, public Identifiable<unsigned int> {
 public:
  Framebuffer();

  ~Framebuffer();

  // 添加渲染缓冲对象附件
  void Attach(const Renderbuffer& renderbuffer) const;

  // 添加纹理附件
  void Attach(const Texture& texture) const;

  void Bind() const override;

  [[nodiscard]] bool IsComplete() const;

  void Unbind() const override;
};
