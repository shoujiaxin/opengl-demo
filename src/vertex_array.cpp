//
// Created by Jiaxin Shou on 2021/9/17.
//

#include "buffer/vertex_array.h"

#include "glad/glad.h"

VertexArray::VertexArray() { glGenVertexArrays(1, &id_); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id_); }

void VertexArray::Bind() const { glBindVertexArray(id_); }

void VertexArray::Bind(const Buffer& buffer) const {
  const auto guard = BindGuard(*this);
  buffer.Bind();
}

void VertexArray::SetAttribute(int index, int size, int stride, int offset) const {
  const auto guard = BindGuard(*this);
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
}

void VertexArray::Unbind() const { glBindVertexArray(0); }
