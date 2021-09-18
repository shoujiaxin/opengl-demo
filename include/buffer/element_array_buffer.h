//
// Created by Jiaxin Shou on 2021/9/18.
//

#pragma once

#include "buffer.h"

class ElementArrayBuffer final : public Buffer {
 public:
  ElementArrayBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER){};

  template <class T>
  explicit ElementArrayBuffer(const std::vector<T>& data)
      : ElementArrayBuffer(&data.front(), data.size() * sizeof(T)) {}

  template <class T>
  ElementArrayBuffer(const T* data, int size) : ElementArrayBuffer() {
    const auto guard = BindGuard(*this);
    glBufferData(target_, size, data, GL_STATIC_DRAW);
    size_ = size;
  }
};
