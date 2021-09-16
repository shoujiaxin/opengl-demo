//
// Created by Jiaxin Shou on 2021/9/14.
//

#include "framebuffer.h"

#include "glad/glad.h"

Framebuffer::Framebuffer() { glGenFramebuffers(1, &id_); }

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &id_); }

void Framebuffer::Attach(const Renderbuffer &renderbuffer) const {
  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                            renderbuffer.Id());
}

void Framebuffer::Attach(const Texture &texture) const {
  Bind();
  switch (texture.Format()) {
    case Texture::Format::kDefault:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.Id(), 0);
      break;
    case Texture::Format::kDepthComponent:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.Id(), 0);
      break;
    case Texture::Format::kDepthStencil:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                             texture.Id(), 0);
      break;
    case Texture::Format::kStencilIndex:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture.Id(), 0);
      break;
    default:
      assert(false);
  }
}

void Framebuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id_); }

bool Framebuffer::IsComplete() const {
  Bind();
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
