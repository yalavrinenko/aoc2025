//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <ranges>
#include <sstream>
#include <string>
#include <vector>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  int sign;
  int value;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    return lines | vw::transform([](auto s) {
             char c;
             std::istringstream iss{s};
             iss >> c;
             int value;
             iss >> value;

             return AOC_Input{.sign = (c == 'L') ? -1 : 1, .value = value};
           }) |
           as_vector;
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  static constexpr auto MaxValue = 100;

  auto rotation_trace(const std::vector<AOC_Input> &v, int init) {
    return v | vw::transform([&init](AOC_Input rotation) {
             init += rotation.sign * (rotation.value % MaxValue);
             if (init < 0) {
               init = MaxValue + init;
             }
             if (init >= MaxValue) {
               init = init % MaxValue;
             }
             return init;
           }) |
           as_vector;
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto trace = rotation_trace(v, 50);
    return AOC_Output(std::ranges::count(trace, 0));
  }

  struct Disc {
    explicit Disc(int init) : position(init) {}

    void roll(int rotation) {
      clicks += std::abs(rotation) / MaxValue;
      rotation %= MaxValue;

      const auto new_position = position + rotation;

      if (new_position < 0 && position != 0) {
        ++clicks;
      }
      if (new_position >= MaxValue) {
        ++clicks;
      }
      if (new_position == 0) {
        ++clicks;
      }

      if (new_position < 0) {
        position = MaxValue + new_position;
      } else if (new_position >= MaxValue) {
        position = new_position % MaxValue;
      } else
        position = new_position;
    }

    int clicks = 0;
    int position;
  };

  auto clicks_count(const std::vector<AOC_Input> &v, int init) {
    Disc d{init};
    for (auto rotation : v) {
      d.roll(rotation.sign * rotation.value);
    }

    return d.clicks;
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    return AOC_Output{clicks_count(v, 50)};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
