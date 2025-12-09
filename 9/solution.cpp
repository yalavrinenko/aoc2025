//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fmt/color.h>
#include <fmt/core.h>
#include <ranges>
#include <string>
#include <vector>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct Point {
  double x, y;
};

struct AOC_Reader {
  static std::vector<Point>
  from_string_vector(const std::vector<std::string> &lines) {
    return lines | vw::transform([](auto const &s) {
             Point p;
             std::sscanf(s.c_str(), "%lf,%lf", &p.x, &p.y);
             return p;
           }) |
           as_vector;
  }
};

class AOCDaySolution : public AOCSolution<AOCDaySolution, Point, AOC_Reader> {
  friend AOCSolution;

  AOC_Output part1(std::vector<Point> const &v) {
    double max_s = 0;
    for (auto i : vw::iota(0UL, v.size() - 1)) {
      for (auto j : vw::iota(i + 1, v.size())) {
        const auto lx = std::abs(v[i].x - v[j].x) + 1;
        const auto ly = std::abs(v[i].y - v[j].y) + 1;
        if (lx * ly > max_s) {
          max_s = lx * ly;
        }
      }
    }
    return AOC_Output{static_cast<uint64_t>(max_s)};
  }

  struct Segment {
    Point a, b;
  };

  bool point_in_rectangle(Point p, Point left_bottom, Point right_up) {
    return p.x > left_bottom.x && p.x < right_up.x && p.y > left_bottom.y &&
           p.y < right_up.y;
  }

  bool segment_intersects_line(Point seg_a, Point seg_b, Point line_a,
                               Point line_b) {
    auto cross = [](Point p1, Point p2, Point p3) {
      return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    };

    Point p1 = seg_a, p2 = seg_b, p3 = line_a, p4 = line_b;

    int d1 = cross(p3, p4, p1);
    int d2 = cross(p3, p4, p2);
    int d3 = cross(p1, p2, p3);
    int d4 = cross(p1, p2, p4);

    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
      return true;
    }

    return false;
  }

  bool vertical_intersection(Point left, Point right, Segment vs) {
    auto top_y = std::max(vs.a.y, vs.b.y);
    auto bottom_y = std::min(vs.a.y, vs.b.y);
    return (left.x < vs.a.x && vs.a.x < right.x) &&
           (bottom_y < left.y && left.y < top_y);
  }

  bool horizontal_intersection(Point bottom, Point top, Segment hs) {
    auto left_x = std::min(hs.a.x, hs.b.x);
    auto right_x = std::max(hs.a.x, hs.b.x);

    return (bottom.y < hs.a.y && hs.a.y < top.y) &&
           (left_x < bottom.x && bottom.x < right_x);
  }

  bool intersect(Point a, Point b, std::vector<Segment> const &segments) {
    const Point left_bottom{.x = std::min(a.x, b.x), .y = std::min(a.y, b.y)};
    const Point left_up{.x = std::min(a.x, b.x), .y = std::max(a.y, b.y)};
    const Point right_bottom{.x = std::max(a.x, b.x), .y = std::min(a.y, b.y)};
    const Point right_up{.x = std::max(a.x, b.x), .y = std::max(a.y, b.y)};

    for (const auto &segment : segments) {
      if (point_in_rectangle(segment.a, left_bottom, right_up) &&
          point_in_rectangle(segment.b, left_bottom, right_up)) {
        return true;
      } else if (point_in_rectangle(segment.a, left_bottom, right_up) &&
                 !point_in_rectangle(segment.b, left_bottom, right_up)) {
        return true;
      } else if (!point_in_rectangle(segment.a, left_bottom, right_up) &&
                 point_in_rectangle(segment.b, left_bottom, right_up)) {
        return true;
      } else {
        if (segment.a.x == segment.b.x) {
          if (vertical_intersection(left_bottom, right_bottom, segment) ||
              vertical_intersection(left_up, right_up, segment)) {
            return true;
          }
        }

        if (segment.a.y == segment.b.y) {
          if (horizontal_intersection(left_bottom, left_up, segment)) {
            return true;
          }
        }
      }
    }

    return false; // No intersections found
  }

  AOC_Output part2(std::vector<Point> const &v) {
    std::vector<Segment> s;
    for (auto i : vw::iota(1UL, v.size())) {
      s.emplace_back(Segment{v[i - 1], v[i]});
    }

    double max_s = 0;
    for (auto i : vw::iota(0UL, v.size() - 1)) {
      for (auto j : vw::iota(i + 1, v.size())) {
        const auto lx = std::abs(v[i].x - v[j].x) + 1;
        const auto ly = std::abs(v[i].y - v[j].y) + 1;
        if (!intersect(v[i], v[j], s) && lx * ly > max_s) {
          max_s = lx * ly;
        }
      }
    }

    return AOC_Output{static_cast<uint64_t>(max_s)};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
