//
// Created by Jiaxin Shou on 2021/9/10.
//

#include "model/model.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "spdlog/spdlog.h"

Model::Model(const std::string &path) {
  directory_ = path.substr(0, path.find_last_of('/'));

  LoadModelFrom(path);
}

void Model::Draw(const Program &program) const {
  program.Use();
  for (const auto &mesh : meshes_) {
    mesh->Draw(program);
  }

  const auto error = glGetError();
  if (error != GL_NO_ERROR) {
    spdlog::error("OpenGL error: {0}", error);
  }
}

std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(const aiMaterial *material,
                                                                  aiTextureType type) {
  auto texture_type = Texture::Type::kDefault;
  switch (type) {
    case aiTextureType_AMBIENT:
      texture_type = Texture::Type::kReflectionMapping;
      break;
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
  std::vector<std::shared_ptr<Texture>> textures;
  textures.reserve(num_textures);
  for (auto i = 0; i < num_textures; ++i) {
    // 相对路径
    aiString path;
    material->GetTexture(type, i, &path);

    // 绝对路径
    const auto texture_path = directory_ + '/' + std::string(path.C_Str());

    if (texture_cache_.find(texture_path) == texture_cache_.cend()) {
      textures.emplace_back(std::make_shared<Texture>(texture_path, texture_type));
      texture_cache_.insert({texture_path, textures.back()});
    } else {
      textures.push_back(texture_cache_[texture_path]);
    }
  }

  return textures;
}

void Model::LoadModelFrom(const std::string &path) {
  Assimp::Importer importer;
  const auto scene = importer.ReadFile(path, aiProcess_Triangulate);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    spdlog::error("failed to load model: {0}\n{1}", path, importer.GetErrorString());
    return;
  }

  ProcessNode(scene, scene->mRootNode);
}

std::shared_ptr<Mesh> Model::ProcessMesh(const aiScene *scene, const aiMesh *mesh) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> textures;

  // 顶点
  const auto num_vertices = mesh->mNumVertices;
  vertices.reserve(num_vertices);
  for (auto i = 0; i < num_vertices; ++i) {
    auto vertex = Vertex();

    vertex.position_ = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

    if (mesh->HasNormals()) {
      vertex.normal_ = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
    }

    if (mesh->mTextureCoords[0]) {
      vertex.texture_coordinates_ =
          glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    vertices.emplace_back(vertex);
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

    // 漫反射贴图
    const auto diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuse_maps.cbegin(), diffuse_maps.cend());

    // 镜面光贴图
    const auto specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specular_maps.cbegin(), specular_maps.cend());

    // 反射贴图
    const auto reflection_maps = LoadMaterialTextures(material, aiTextureType_AMBIENT);
    textures.insert(textures.end(), reflection_maps.cbegin(), reflection_maps.cend());
  }

  return std::make_shared<Mesh>(vertices, indices, textures);
}

void Model::ProcessNode(const aiScene *scene, const aiNode *node) {
  for (auto i = 0; i < node->mNumMeshes; ++i) {
    const auto mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.emplace_back(std::move(ProcessMesh(scene, mesh)));
  }

  for (auto i = 0; i < node->mNumChildren; ++i) {
    ProcessNode(scene, node->mChildren[i]);
  }
}

std::unordered_map<std::string, std::shared_ptr<Texture>> Model::texture_cache_{};
