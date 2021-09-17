//
// Created by Jiaxin Shou on 2021/9/16.
//

#pragma once

#include <vector>

#include "glad/glad.h"
#include "interface/bindable.h"

class Buffer : public Bindable {
 public:
  explicit Buffer(GLenum target);

  virtual ~Buffer();

  void Bind() const override;

  // 复制缓冲内容
  void CopyData(const Buffer& source);

  [[nodiscard]] unsigned int Id() const;

  void SetData(const void* data, int size);

  template <class T>
  void SetData(const std::vector<T>& data);

  void SetSubData(const void* data, int size, int offset) const;

  [[nodiscard]] int Size() const;

  void Unbind() const override;

 protected:
  unsigned int id_ = 0;

  int size_ = 0;

  GLenum target_ = 0;
};

template <class T>
void Buffer::SetData(const std::vector<T>& data) {
  SetData(&data.front(), sizeof(T) * data.size());
}
