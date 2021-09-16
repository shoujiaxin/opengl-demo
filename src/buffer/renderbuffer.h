//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

#include "../util/bindable.h"

class Renderbuffer final : public Bindable {
 public:
  Renderbuffer(int width, int height);

  ~Renderbuffer();

  void Bind() const override;

  [[nodiscard]] int Height() const;

  [[nodiscard]] unsigned int Id() const;

  void Unbind() const override;

  [[nodiscard]] int Width() const;

 private:
  int height_ = 0;

  unsigned int id_ = 0;

  int width_ = 0;
};
