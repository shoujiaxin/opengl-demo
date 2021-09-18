//
// Created by Jiaxin Shou on 2021/9/17.
//

#pragma once

#include "buffer.h"

class UniformBuffer final : public Buffer {
 public:
  UniformBuffer() : Buffer(GL_UNIFORM_BUFFER) {}

  explicit UniformBuffer(int size) : UniformBuffer() {
    const auto guard = BindGuard(*this);
    glBufferData(target_, size, nullptr, GL_STATIC_DRAW);
    size_ = size;
  }

  void BindTo(int binding_point) const { glBindBufferBase(target_, binding_point, id_); }

  void BindTo(int binding_point, int offset, int size) const {
    glBindBufferRange(target_, binding_point, id_, offset, size);
  }
};
