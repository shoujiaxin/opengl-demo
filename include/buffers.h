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
    Bind();
    glBufferData(target_, size, data, GL_STATIC_DRAW);
    size_ = size;
  }
};
