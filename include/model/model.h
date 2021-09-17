//
// Created by Jiaxin Shou on 2021/9/10.
//

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "assimp/scene.h"
#include "mesh/mesh.h"
#include "shader/program.h"

class Model final {
 public:
  explicit Model(const std::string& path);

  void Draw(const Program& program) const;

 private:
  std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(const aiMaterial* material,
                                                             aiTextureType type);

  void LoadModelFrom(const std::string& path);

  std::shared_ptr<Mesh> ProcessMesh(const aiScene* scene, const aiMesh* mesh);

  void ProcessNode(const aiScene* scene, const aiNode* node);

  std::string directory_;

  std::vector<std::shared_ptr<Mesh>> meshes_;

  // 纹理缓存
  static std::unordered_map<std::string, std::shared_ptr<Texture>> texture_cache_;
};
