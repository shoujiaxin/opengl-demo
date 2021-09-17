//
// Created by Jiaxin Shou on 2021/9/17.
//

#pragma once

template <class T>
class Identifiable {
 public:
  explicit Identifiable(T id = T()) : id_(id) {}

  T Id() const { return id_; }

 protected:
  T id_ = T();
};
