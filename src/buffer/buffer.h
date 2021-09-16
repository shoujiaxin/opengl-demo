//
// Created by Jiaxin Shou on 2021/9/16.
//

#pragma once

#include <vector>

#include "../util/bindable.h"
#include "glad/glad.h"

class Buffer : public Bindable {
 public:
  explicit Buffer(GLenum target);

  virtual ~Buffer();

  void Bind() const override;

  // 复制缓冲内容
  void CopyData(const Buffer& source);

  [[nodiscard]] unsigned int Id() const;

  template <class T>
  void SetData(const T* data, int size);

  void Unbind() const override;

 protected:
  unsigned int id_ = 0;

  int size_ = 0;

  GLenum target_ = 0;
};

template <class T>
void Buffer::SetData(const T* data, int size) {
  const auto guard = BindGuard(*this);
  glBufferData(target_, size, data, GL_STATIC_DRAW);
  size_ = size;
}
