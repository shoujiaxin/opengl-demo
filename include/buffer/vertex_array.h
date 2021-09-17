//
// Created by Jiaxin Shou on 2021/9/17.
//

#pragma once

#include "array_buffer.h"
#include "interface/bindable.h"

class VertexArray final : public Bindable {
 public:
  VertexArray();

  ~VertexArray();

  void Bind() const override;

  void Bind(const Buffer& buffer) const;

  void SetAttribute(int index, int size, int stride, int offset) const;

  void Unbind() const override;

 private:
  unsigned int id_ = 0;
};
