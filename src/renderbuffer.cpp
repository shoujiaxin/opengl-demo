//
// Created by Jiaxin Shou on 2021/9/14.
//

#include "buffer/renderbuffer.h"

#include "glad/glad.h"

Renderbuffer::Renderbuffer(int width, int height) : height_(height), width_(width) {
  glGenRenderbuffers(1, &id_);
  const auto guard = BindGuard(*this);

  // 创建深度和模板渲染缓冲
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

Renderbuffer::~Renderbuffer() { glDeleteRenderbuffers(1, &id_); }

void Renderbuffer::Bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id_); }

int Renderbuffer::Height() const { return height_; }

unsigned int Renderbuffer::Id() const { return id_; }

void Renderbuffer::Unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

int Renderbuffer::Width() const { return width_; }
