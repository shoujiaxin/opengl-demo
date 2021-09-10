//
// Created by Jiaxin Shou on 2021/9/10.
//

#include "model.h"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

Model::Model(const std::string &path) {
  directory_ = path.substr(0, path.find_last_of('/'));

  LoadModelFrom(path);
}

void Model::Draw(const Program &program) const {
  for (const auto &mesh : meshes_) {
    mesh.Draw(program);
  }
}

std::vector<Texture> Model::LoadMaterialTextures(const aiMaterial *material, aiTextureType type) {
  auto texture_type = Texture::Type::kDefault;
  switch (type) {
    case aiTextureType_DIFFUSE:
      texture_type = Texture::Type::kDiffuseMapping;
      break;
    case aiTextureType_SPECULAR:
      texture_type = Texture::Type::kSpecularMapping;
      break;
    default:
      break;
  }

  const auto num_textures = material->GetTextureCount(type);
  std::vector<Texture> textures;
  textures.reserve(num_textures);
  for (auto i = 0; i < num_textures; ++i) {
    // 相对路径
    aiString path;
    material->GetTexture(type, i, &path);

    // 绝对路径
    const auto texture_path = directory_ + std::string(path.C_Str());

    if (texture_cache_.find(texture_path) == texture_cache_.cend()) {
      textures.emplace_back(texture_path, texture_type);
      texture_cache_.insert({texture_path, textures.back()});
    } else {
      textures.push_back(texture_cache_[texture_path]);
    }
  }

  return textures;
}

void Model::LoadModelFrom(const std::string &path) {
  Assimp::Importer importer;
  const auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "Failed to load model: " << path << std::endl;
    std::cerr << importer.GetErrorString() << std::endl;
    return;
  }

  ProcessNode(scene, scene->mRootNode);
}

Mesh Model::ProcessMesh(const aiScene *scene, const aiMesh *mesh) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  // 顶点
  const auto num_vertices = mesh->mNumVertices;
  vertices.reserve(num_vertices);
  for (auto i = 0; i < num_vertices; ++i) {
    const auto position =
        glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    const auto normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
    const auto texture_coordinates =
        (mesh->mTextureCoords[0])
            ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
            : glm::vec2(0.0f, 0.0f);

    vertices.emplace_back(position, normal, texture_coordinates);
  }

  // 索引
  for (auto i = 0; i < mesh->mNumFaces; ++i) {
    const auto face = mesh->mFaces[i];
    for (auto j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // 材质
  if (mesh->mMaterialIndex >= 0) {
    const auto material = scene->mMaterials[mesh->mMaterialIndex];

    const auto diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuse_maps.cbegin(), diffuse_maps.cend());

    const auto specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specular_maps.cbegin(), specular_maps.cend());
  }

  return {vertices, indices, textures};
}

void Model::ProcessNode(const aiScene *scene, const aiNode *node) {
  for (auto i = 0; i < node->mNumMeshes; ++i) {
    const auto mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(ProcessMesh(scene, mesh));
  }

  for (auto i = 0; i < node->mNumChildren; ++i) {
    ProcessNode(scene, node->mChildren[i]);
  }
}

std::unordered_map<std::string, Texture> Model::texture_cache_{};
