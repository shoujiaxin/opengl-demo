//
// Created by Jiaxin Shou on 2021/9/9.
//

#include "mesh.h"

#include <string>
#include <utility>

#include "glad/glad.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<std::shared_ptr<Texture>> textures)
    : vertices_(std::move(vertices)), indices_(std::move(indices)), textures_(std::move(textures)) {
  Setup();
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vertex_array_object_);
  glDeleteBuffers(1, &vertex_buffer_object_);
  glDeleteBuffers(1, &element_buffer_object_);
}

void Mesh::Draw(const Program &program) const {
  auto diffuse_cnt = 0;
  auto reflection_cnt = 0;
  auto specular_cnt = 0;

  for (const auto &texture : textures_) {
    std::string name;
    switch (texture->Type()) {
      case Texture::Type::kDiffuseMapping:
        name = "texture_diffuse" + std::to_string(++diffuse_cnt);
        break;
      case Texture::Type::kReflectionMapping:
        name = "texture_reflection" + std::to_string(++reflection_cnt);
        break;
      case Texture::Type::kSpecularMapping:
        name = "texture_specular" + std::to_string(++specular_cnt);
        break;
      default:
        break;
    }

    program.SetUniform("material." + name, static_cast<int>(texture->Id()));
    glActiveTexture(GL_TEXTURE0 + texture->Id());
    texture->Bind();
  }

  glActiveTexture(GL_TEXTURE0);

  // 绘制
  glBindVertexArray(vertex_array_object_);
  glDrawElements(GL_TRIANGLES, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void Mesh::Setup() {
  glGenVertexArrays(1, &vertex_array_object_);
  glGenBuffers(1, &vertex_buffer_object_);
  glGenBuffers(1, &element_buffer_object_);

  glBindVertexArray(vertex_array_object_);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
  glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices_.size() * sizeof(Vertex)), &vertices_[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices_.size() * sizeof(unsigned int)),
               &indices_[0], GL_STATIC_DRAW);

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
