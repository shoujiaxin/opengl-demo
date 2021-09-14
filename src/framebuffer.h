//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

#include "renderbuffer.h"
#include "texture.h"

class Framebuffer {
 public:
  Framebuffer();

  ~Framebuffer();

  // 添加渲染缓冲对象附件
  void Attach(const Renderbuffer& renderbuffer) const;

  // 添加纹理附件
  void Attach(const Texture& texture) const;

  void Bind() const;

  [[nodiscard]] bool IsComplete() const;

 private:
  unsigned int id_ = 0;
};
