//
// Created by Jiaxin Shou on 2021/9/17.
//

#pragma once

#include "buffer.h"

class ArrayBuffer final : public Buffer {
 public:
  ArrayBuffer() : Buffer(GL_ARRAY_BUFFER) {}

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
