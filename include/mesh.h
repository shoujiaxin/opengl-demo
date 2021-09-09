//
// Created by Jiaxin Shou on 2021/9/9.
//

#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "program.h"
#include "texture.h"

struct Vertex {
  // 法向量
  glm::vec3 normal_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 位置
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);

  // 纹理坐标
  glm::vec2 texture_coordinates_ = glm::vec2(0.0f, 0.0f);
};

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
       const std::vector<Texture>& textures);

  ~Mesh();

  void Draw(const Program& program);

 private:
  void Setup();

  // 索引缓冲对象
  unsigned int element_buffer_object_ = 0;

  // 索引
  std::vector<unsigned int> indices_ = {};

  // 纹理
  std::vector<Texture> textures_ = {};

  // 顶点
  std::vector<Vertex> vertices_ = {};

  // 顶点数组对象
  unsigned int vertex_array_object_ = 0;

  // 顶点缓冲对象
  unsigned int vertex_buffer_object_ = 0;
};
