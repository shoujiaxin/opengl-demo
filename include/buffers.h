//
// Created by Jiaxin Shou on 2021/9/16.
//

#pragma once

#include "../src/buffer/buffer.h"
#include "../src/buffer/framebuffer.h"
#include "../src/buffer/renderbuffer.h"

class ArrayBuffer final : public Buffer {
 public:
  ArrayBuffer() : Buffer(GL_ARRAY_BUFFER){};

  template <class T>
  explicit ArrayBuffer(const std::vector<T>& data)
      : ArrayBuffer(&data.front(), data.size() * sizeof(T)) {}

  template <class T>
  ArrayBuffer(const T* data, int size) : ArrayBuffer() {
    const auto guard = BindGuard(*this);
    glBufferData(target_, size, data, GL_STATIC_DRAW);
    size_ = size;
  }
};

class UniformBuffer final : public Buffer {
 public:
  UniformBuffer() : Buffer(GL_UNIFORM_BUFFER) {}

  explicit UniformBuffer(int size) : UniformBuffer() {
    const auto guard = BindGuard(*this);
    glBufferData(target_, size, nullptr, GL_STATIC_DRAW);
    size_ = size;
  }

  void Bind(int binding_point) const { glBindBufferBase(target_, binding_point, id_); }

  void Bind(int binding_point, int offset, int size) const {
    glBindBufferRange(target_, binding_point, id_, offset, size);
  }
};
