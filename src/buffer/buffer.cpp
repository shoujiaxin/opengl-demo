//
// Created by Jiaxin Shou on 2021/9/16.
//

#include "buffer.h"

Buffer::Buffer(Target target) : target_(target) { glGenBuffers(1, &id_); }

Buffer::~Buffer() { glDeleteBuffers(1, &id_); }

void Buffer::Bind() const {
  switch (target_) {
    case Target::kArrayBuffer:
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      break;
    default:
      break;
  }
}

unsigned int Buffer::Id() const { return id_; }
