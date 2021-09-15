//
// Created by Jiaxin Shou on 2021/9/9.
//

#include "mesh.h"

#include <string>

#include "glad/glad.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
           const std::vector<std::shared_ptr<Texture>> &textures)
    : vertices_(vertices), indices_(indices), textures_(textures) {
  Setup();
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vertex_array_object_);
  glDeleteBuffers(1, &vertex_buffer_object_);
  glDeleteBuffers(1, &element_buffer_object_);
}

void Mesh::Draw(const Program &program) const {
  auto diffuse_cnt = 1;
  auto specular_cnt = 1;

  for (auto i = 0; i < textures_.size(); ++i) {
    const auto type = textures_[i]->Type();
    std::string name;
    if (type == Texture::Type::kDiffuseMap) {
      name = "texture_diffuse" + std::to_string(diffuse_cnt++);
    } else if (type == Texture::Type::kSpecularMap) {
      name = "texture_specular" + std::to_string(specular_cnt++);
    }

    program.SetUniform("material." + name, static_cast<int>(textures_[i]->Id()));
    glActiveTexture(GL_TEXTURE0 + textures_[i]->Id());
    textures_[i]->Bind();
  }

  glActiveTexture(GL_TEXTURE0);

  // 绘制
  glBindVertexArray(vertex_array_object_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void Mesh::Setup() {
  glGenVertexArrays(1, &vertex_array_object_);
  glGenBuffers(1, &vertex_buffer_object_);
  glGenBuffers(1, &element_buffer_object_);

  glBindVertexArray(vertex_array_object_);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0],
               GL_STATIC_DRAW);

  // 位置
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, position_)));
  // 法向量
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, normal_)));
  // 纹理坐标
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, texture_coordinates_)));

  glBindVertexArray(0);
}
