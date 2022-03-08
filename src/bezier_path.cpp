//
// Created by Jiaxin Shou on 2022/3/8.
//

#include "util/bezier_path.h"

#include <cmath>

void BezierPath::MoveTo(const Point& point) { points_.emplace_back(point); }

void BezierPath::AddCurveTo(const Point& end_point, const Point& control_point) {
  if (points_.empty()) {
    return;
  }

  // 起始点
  const auto& p0 = points_.back();
  // 控制点
  const auto& p1 = control_point;
  // 终止点
  const auto& p2 = end_point;

  // 匀速贝塞尔曲线系数
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

    const auto temp1 = sqrt(C + t * (B + A * t));
    const auto temp2 = (2 * A * t * temp1 + B * (temp1 - sqrt(C)));
    const auto temp3 = log(abs(B + 2 * sqrt(A) * sqrt(C) + 0.0001));
    const auto temp4 = log(abs(B + 2 * A * t + 2 * sqrt(A) * temp1) + 0.0001);
    const auto temp5 = 2 * sqrt(A) * temp2;
    const auto temp6 = (B * B - 4 * A * C) * (temp3 - temp4);

    return (temp5 + temp6) / (8 * pow(A, 1.5));
  };

  // 速度函数
  const auto curve_speed = [&](double t) -> double {
    return sqrt(std::max(A * pow(t, 2.0) + B * t + C, 0.0));
  };

  // 长度计算函数的反函数 L^-1(t)
  const auto linear_t = [&](double t, double length) -> double {
    auto t1 = t;
    decltype(t) t2;

    while (true) {
      const auto speed = curve_speed(t1);
      if (speed < 1.0) {
        t2 = t1;
        break;
      }
      t2 = t1 - (curve_speed(t1) - length) / speed;
      if (abs(t1 - t2) < 1.0) {
        break;
      }
      t1 = t2;
    }
    return t2;
  };

  // 计算 p0 到 p2 的曲线长度
  const auto total_length = curve_length(1.0);

  // 插值点数量
  const auto count = std::max(1, static_cast<int>(ceil(total_length * 200)));

  for (auto i = 1; i <= count; ++i) {
    auto t = static_cast<double>(i) / count;
    const auto length = t * total_length;
    t = linear_t(t, length);

    const auto x = (1 - t) * (1 - t) * p0.x_ + 2 * t * (1 - t) * p1.x_ + t * t * p2.x_;
    const auto y = (1 - t) * (1 - t) * p0.y_ + 2 * t * (1 - t) * p1.y_ + t * t * p2.y_;
    points_.emplace_back(x, y);
  }
}

const std::vector<Point>& BezierPath::GetPoints() const { return points_; }
