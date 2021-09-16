//
// Created by Jiaxin Shou on 2021/9/16.
//

#include "buffer.h"

Buffer::Buffer(GLenum target) : target_(target) { glGenBuffers(1, &id_); }

Buffer::~Buffer() { glDeleteBuffers(1, &id_); }

void Buffer::Bind() const { glBindBuffer(target_, id_); }

void Buffer::CopyData(const Buffer &source) {
  auto read_target = source.target_;
  auto write_target = target_;

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

void Buffer::Unbind() const { glBindBuffer(target_, 0); }
