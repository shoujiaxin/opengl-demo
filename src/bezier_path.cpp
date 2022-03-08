//
// Created by Jiaxin Shou on 2022/3/8.
//

#include "util/bezier_path.h"

#include <cmath>

void BezierPath::AddCurveTo(const Point& end_point, const Point& control_point) {
  if (points_.empty()) {
    points_.emplace_back(end_point);
    return;
  }

  // 起始点
  const auto& p0 = points_.back();
  // 控制点
  const auto& p1 = control_point;
  // 终止点
  const auto& p2 = end_point;

  const auto ax = p0.x_ - 2 * p1.x_ + p2.x_;
  const auto ay = p0.y_ - 2 * p1.y_ + p2.y_;
  const auto bx = 2 * p1.x_ - 2 * p0.x_;
  const auto by = 2 * p1.y_ - 2 * p0.y_;

  const auto A = 4 * (ax * ax + ay * ay);
  const auto B = 4 * (ax * bx + ay * by);
  const auto C = bx * bx + by * by;

  // 长度计算函数 L(t)
  const auto curve_length = [&](double t) -> double {
    if (A < 0.00001f) {
      return 0.0f;
    }

    float temp1 = sqrtf(C + t * (B + A * t));
    float temp2 = (2 * A * t * temp1 + B * (temp1 - sqrtf(C)));
    float temp3 = log(abs(B + 2 * sqrtf(A) * sqrtf(C) + 0.0001f));
    float temp4 = log(abs(B + 2 * A * t + 2 * sqrtf(A) * temp1) + 0.0001f);
    float temp5 = 2 * sqrtf(A) * temp2;
    float temp6 = (B * B - 4 * A * C) * (temp3 - temp4);

    return (temp5 + temp6) / (8 * powf(A, 1.5f));
  };

  // 速度函数
  const auto curve_speed = [&](double t) -> double {
    return sqrtf(std::max(A * pow(t, 2.0) + B * t + C, 0.0));
  };

  // 长度计算函数的反函数 L^-1(t)
  const auto curve_length_inverse = [&](double t, double length) -> double {
    float t1 = t;
    float t2;

    while (true) {
      float speed = curve_speed(t1);
      if (speed < 0.0001f) {
        t2 = t1;
        break;
      }
      t2 = t1 - (curve_speed(t1) - length) / speed;
      if (abs(t1 - t2) < 0.0001f) {
        break;
      }
      t1 = t2;
    }
    return t2;
  };

  // 计算 p0 到 p2 的曲线长度
  const auto total_length = curve_length(1.0);

  // 插值点数量
  const auto count = ceil(total_length / 20);

  for (auto i = 1; i <= count; ++i) {
    auto t = static_cast<double>(i) / count;
    const auto length = t * total_length;
    t = curve_length_inverse(t, length);

    const auto x = (1 - t) * (1 - t) * p0.x_ + 2 * t * (1 - t) * p1.x_ + t * t * p2.x_;
    const auto y = (1 - t) * (1 - t) * p0.y_ + 2 * t * (1 - t) * p1.y_ + t * t * p2.y_;
    points_.emplace_back(x, y);
  }
}

const std::vector<Point>& BezierPath::GetPoints() const { return points_; }
