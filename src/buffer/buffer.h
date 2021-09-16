//
// Created by Jiaxin Shou on 2021/9/16.
//

#pragma once

#include <vector>

#include "glad/glad.h"

class Buffer {
 public:
  enum class Target { kArrayBuffer };

  explicit Buffer(Target target);

  template <class T>
  Buffer(Target target, const std::vector<T>& data);

  template <class T>
  Buffer(Target target, const T* data, int size);

  ~Buffer();

  void Bind() const;

  // 复制缓冲内容
  void CopyDataFrom(const Buffer& source);

  [[nodiscard]] unsigned int Id() const;

  template <class T>
  void SetData(const T* data, int size);

 private:
  [[nodiscard]] GLenum BindingTarget() const;

  unsigned int id_ = 0;

  int size_ = 0;

  Target target_ = Target::kArrayBuffer;
};

template <class T>
Buffer::Buffer(Target target, const T* data, int size) : Buffer(target) {
  SetData(data, size);
}

template <class T>
Buffer::Buffer(Target target, const std::vector<T>& data)
    : Buffer(target, &data.front(), data.size() * sizeof(T)) {}

template <class T>
void Buffer::SetData(const T* data, int size) {
  Bind();
  glBufferData(BindingTarget(), size, data, GL_STATIC_DRAW);
  size_ = size;
}
