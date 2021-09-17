//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

#include "interface/bindable.h"
#include "interface/identifiable.h"

class Renderbuffer final : public Bindable, public Identifiable<unsigned int> {
 public:
  Renderbuffer(int width, int height);

  ~Renderbuffer();

  void Bind() const override;

  [[nodiscard]] int Height() const;

  void Unbind() const override;

  [[nodiscard]] int Width() const;

 private:
  int height_ = 0;

  int width_ = 0;
};
