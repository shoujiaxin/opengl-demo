//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

#include "../src/texture.h"
#include "../util/bindable.h"
#include "renderbuffer.h"

class Framebuffer final : public Bindable {
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

 private:
  unsigned int id_ = 0;
};
