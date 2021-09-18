//
// Created by Jiaxin Shou on 2021/9/9.
//

#include "mesh/mesh.h"

#include <string>
#include <utility>

#include "glad/glad.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<std::shared_ptr<Texture>> textures)
    : vertices_(std::move(vertices)),
      indices_(std::move(indices)),
      textures_(std::move(textures)),
      vertex_array_object_(std::make_shared<VertexArray>()),
      vertex_buffer_object_(std::make_shared<ArrayBuffer>(vertices_)),
      element_buffer_object_(std::make_shared<ElementArrayBuffer>(indices_)) {
  const auto guard = BindGuard(*vertex_array_object_);

  vertex_array_object_->Bind(*vertex_buffer_object_);
  vertex_array_object_->Bind(*element_buffer_object_);

  // 位置
  vertex_array_object_->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position_));
  // 法向量
  vertex_array_object_->SetAttribute(1, 3, sizeof(Vertex), offsetof(Vertex, normal_));
  // 纹理坐标
  vertex_array_object_->SetAttribute(2, 2, sizeof(Vertex), offsetof(Vertex, texture_coordinates_));
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

    // 绑定纹理到纹理单元
    texture->BindToUnit(static_cast<int>(texture->Id()));
    // 将纹理单元设置给采样器
    program.SetUniform("material." + name, static_cast<int>(texture->Id()));
  }

  // 绘制
  const auto guard = BindGuard(*vertex_array_object_);
  glDrawElements(GL_TRIANGLES, static_cast<int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
}
