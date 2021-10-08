//
// Created by Jiaxin Shou on 2021/9/9.
//

#pragma once

#include <memory>
#include <vector>

#include "buffer/array_buffer.h"
#include "buffer/element_array_buffer.h"
#include "buffer/vertex_array.h"
#include "glm/glm.hpp"
#include "shader/program.h"
#include "texture/texture.h"

struct Vertex final {
  // 法向量
  glm::vec3 normal_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 位置
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 纹理坐标
  glm::vec2 texture_coordinates_ = glm::vec2(0.0f, 0.0f);
};

class Mesh final {
 public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<std::shared_ptr<Texture>> textures);

  void Draw(const Program& program) const;

 private:
  // 顶点
  std::vector<Vertex> vertices_ = {};

  // 索引
  std::vector<unsigned int> indices_ = {};

  // 纹理
  std::vector<std::shared_ptr<Texture>> textures_ = {};

  // 顶点数组对象
  std::unique_ptr<VertexArray> vertex_array_object_ = nullptr;

  // 顶点缓冲对象
  std::unique_ptr<ArrayBuffer> vertex_buffer_object_ = nullptr;

  // 索引缓冲对象
  std::unique_ptr<ElementArrayBuffer> element_buffer_object_ = nullptr;
};
