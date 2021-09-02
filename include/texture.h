//
// Created by Jiaxin Shou on 2021/8/22.
//

#pragma once

#include <string>

class Texture {
 public:
  explicit Texture(const std::string& path);

  ~Texture();

  // 绑定纹理
  void Bind() const;

  // 设置过滤方式
  void SetFiltering(int operation, int method) const;

  // 设置环绕方式
  void SetWrap(int axis, int mode) const;

 private:
  int height_ = 0;

  unsigned int id_ = 0;

  int width_ = 0;
};
