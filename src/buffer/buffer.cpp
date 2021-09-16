//
// Created by Jiaxin Shou on 2021/9/16.
//

#include "buffer.h"

Buffer::Buffer(Target target) : target_(target) { glGenBuffers(1, &id_); }

Buffer::~Buffer() { glDeleteBuffers(1, &id_); }

void Buffer::Bind() const { glBindBuffer(BindingTarget(), id_); }

void Buffer::CopyDataFrom(const Buffer &source) {
  auto read_target = source.BindingTarget();
  auto write_target = BindingTarget();

  if (target_ == source.target_) {
    read_target = GL_COPY_READ_BUFFER;
    write_target = GL_COPY_WRITE_BUFFER;
    glBindBuffer(read_target, source.id_);
    glBindBuffer(write_target, id_);
  }

  glCopyBufferSubData(read_target, write_target, 0, 0, source.size_);
  size_ = source.size_;
}

unsigned int Buffer::Id() const { return id_; }

GLenum Buffer::BindingTarget() const {
  switch (target_) {
    case Target::kArrayBuffer:
      return GL_ARRAY_BUFFER;
    default:
      assert(false);
  }
}
