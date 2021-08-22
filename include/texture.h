//
// Created by Jiaxin Shou on 2021/8/22.
//

#pragma once

#include <string>

class Texture {
 public:
  explicit Texture(const std::string& path);

  // 绑定纹理
  void Bind() const;

 private:
  int height_;

  unsigned int id_;

  int width_;
};
