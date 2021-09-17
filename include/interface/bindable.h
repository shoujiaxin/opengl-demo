//
// Created by Jiaxin Shou on 2021/9/17.
//

#pragma once

class Bindable {
 public:
  virtual void Bind() const = 0;

  virtual void Unbind() const = 0;
};

class BindGuard {
 public:
  explicit BindGuard(const Bindable& obj) : obj_(obj) { obj_.Bind(); }

  ~BindGuard() { obj_.Unbind(); }

 private:
  const Bindable& obj_;
};
