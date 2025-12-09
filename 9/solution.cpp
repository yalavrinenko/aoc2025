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

  AOC_Output part2(std::vector<Point> const &v) { return AOC_Output{v.size()}; }
};

AOC_DAY_SOLUTION(AOCDaySolution);
