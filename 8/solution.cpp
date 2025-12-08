//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fmt/color.h>
#include <fmt/core.h>
#include <map>
#include <ranges>
#include <string>

#include <vector>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  double x, y, z;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {

    return lines | vw::transform([](auto const &s) {
             AOC_Input coord;
             std::sscanf(s.data(), "%lf,%lf,%lf", &coord.x, &coord.y, &coord.z);
             return coord;
           }) |
           as_vector;
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  double distance(AOC_Input lhs, AOC_Input rhs) {
    auto pow = [](auto x) { return x * x; };
    return std::sqrt(pow(lhs.x - rhs.x) + pow(lhs.y - rhs.y) +
                     pow(lhs.z - rhs.z));
  }

  struct Point {
    uint64_t i;
    uint64_t j;
    double d;
  };

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    std::vector<Point> pairs;

    for (auto i = 0UL; i < v.size() - 1; ++i) {
      for (auto j = i + 1; j < v.size(); ++j) {
        pairs.emplace_back(i, j, distance(v[i], v[j]));
      }
    }

    std::ranges::sort(pairs, {}, &Point::d);

    std::vector<uint64_t> boxes(v.size(), 0);

    uint64_t box_id = 1;

    static constexpr auto limit = 1000;
    for (auto &[i, j, d] : pairs | vw::take(limit)) {
      if (boxes[i] == 0 && boxes[j] == 0) {
        boxes[i] = boxes[j] = box_id++;
      } else if (boxes[i] != 0 && boxes[j] == 0) {
        boxes[j] = boxes[i];
      } else if (boxes[i] == 0 && boxes[j] != 0) {
        boxes[i] = boxes[j];
      } else {
        auto group_id = std::min(boxes[i], boxes[j]);
        auto ga = boxes[i];
        auto gb = boxes[j];
        if (ga != group_id) {
          std::ranges::replace(boxes, ga, group_id);
        }
        if (gb != group_id) {
          std::ranges::replace(boxes, gb, group_id);
        }
      }
    }

    std::map<int, int> connections;
    for (auto c : boxes) {
      if (c != 0) {
        ++connections[c];
      }
    }

    std::vector<uint64_t> box_sizes;
    for (auto [k, v] : connections) {
      box_sizes.emplace_back(v);
    }

    std::ranges::sort(box_sizes);

    auto dot = 1UL;
    for (auto v : box_sizes | vw::reverse | vw::take(3)) {
      dot *= v;
    }

    return AOC_Output{dot};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    std::vector<Point> pairs;

    for (auto i = 0UL; i < v.size() - 1; ++i) {
      for (auto j = i + 1; j < v.size(); ++j) {
        pairs.emplace_back(i, j, distance(v[i], v[j]));
      }
    }

    std::ranges::sort(pairs, {}, &Point::d);

    std::vector<uint64_t> boxes(v.size(), 0);

    uint64_t box_id = 1;

    for (auto &[i, j, d] : pairs) {
      if (boxes[i] == 0 && boxes[j] == 0) {
        boxes[i] = boxes[j] = box_id++;
      } else if (boxes[i] != 0 && boxes[j] == 0) {
        boxes[j] = boxes[i];
      } else if (boxes[i] == 0 && boxes[j] != 0) {
        boxes[i] = boxes[j];
      } else {
        auto group_id = std::min(boxes[i], boxes[j]);
        auto ga = boxes[i];
        auto gb = boxes[j];
        if (ga != group_id) {
          std::ranges::replace(boxes, ga, group_id);
        }
        if (gb != group_id) {
          std::ranges::replace(boxes, gb, group_id);
        }
      }
      auto front = boxes.front();
      if (std::ranges::all_of(boxes, [front](auto a) { return a == front; })) {
        return AOC_Output{static_cast<uint64_t>(v[i].x * v[j].x)};
      }
    }

    return AOC_Output{};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
