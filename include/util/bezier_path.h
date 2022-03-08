//
// Created by Jiaxin Shou on 2022/3/8.
//

#pragma once

#include <vector>

struct Point {
  Point(double x, double y) : x_(x), y_(y) {}

  double x_;
  double y_;
};

class BezierPath final {
 public:
  void AddCurveTo(const Point& end_point, const Point& control_point);

  const std::vector<Point>& GetPoints() const;

 private:
  std::vector<Point> points_;
};
