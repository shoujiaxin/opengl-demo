//
// Created by Jiaxin Shou on 2021/9/16.
//

#pragma once

#include <vector>

#include "glad/glad.h"

class Buffer {
 public:
  explicit Buffer(GLenum target);

  template <class T>
  explicit Buffer(const std::vector<T>& data);

  template <class T>
  Buffer(const T* data, int size);

  virtual ~Buffer();

  void Bind() const;

  // 复制缓冲内容
  void CopyData(const Buffer& source);

  [[nodiscard]] unsigned int Id() const;

  template <class T>
  void SetData(const T* data, int size);

 protected:
  unsigned int id_ = 0;

  int size_ = 0;

  GLenum target_ = 0;
};

template <class T>
Buffer::Buffer(const std::vector<T>& data) : Buffer(&data.front(), data.size() * sizeof(T)) {}

template <class T>
Buffer::Buffer(const T* data, int size) : Buffer(target_) {
  SetData(data, size);
}

template <class T>
void Buffer::SetData(const T* data, int size) {
  Bind();
  glBufferData(target_, size, data, GL_STATIC_DRAW);
  size_ = size;
}
