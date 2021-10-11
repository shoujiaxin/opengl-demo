//
// Created by Jiaxin Shou on 2021/7/21.
//

#pragma once

#include "glm/glm.hpp"
#include "interface/identifiable.h"
#include "shader.h"

class Program final : public Identifiable<unsigned int> {
 public:
  Program();

  Program(const std::string& vertex_shader, const std::string& fragment_shader);

  Program(const std::string& vertex_shader, const std::string& geometry_shader,
          const std::string& fragment_shader);

  ~Program();

  // 添加着色器
  void AttachShader(const Shader& shader) const;

  void BindUniformBlock(const std::string& name, int binding_point) const;

  // 连接着色器
  void Link() const;

  void SetUniform(const std::string& name, bool value) const;

  void SetUniform(const std::string& name, float value) const;

  void SetUniform(const std::string& name, double value) const;

  void SetUniform(const std::string& name, int value) const;

  void SetUniform(const std::string& name, const glm::mat4& value) const;

  void SetUniform(const std::string& name, const glm::vec3& value) const;

  // 使用着色器程序
  void Use() const;
};
