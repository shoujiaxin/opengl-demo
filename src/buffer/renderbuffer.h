//
// Created by Jiaxin Shou on 2021/9/14.
//

#pragma once

class Renderbuffer final {
 public:
  Renderbuffer(int width, int height);

  ~Renderbuffer();

  void Bind() const;

  [[nodiscard]] int Height() const;

  [[nodiscard]] unsigned int Id() const;

  [[nodiscard]] int Width() const;

 private:
  int height_ = 0;

  unsigned int id_ = 0;

  int width_ = 0;
};
