//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

#include <array>
#include <cstdint>
#include <fmt/color.h>
#include <fmt/core.h>

#include <sstream>
#include <string>
#include <vector>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  int width;
  int height;
  std::vector<int> values;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    std::vector<AOC_Input> result;
    for (const auto &line : lines) {
      AOC_Input input;

      std::istringstream iss{line};
      char x, colon;

      iss >> input.width >> x >> input.height >> colon;

      int val;
      while (iss >> val) {
        input.values.push_back(val);
      }

      result.push_back(input);
    }
    return result;
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    std::array<std::pair<int, int>, 6> shapes{
        std::pair{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3},
    };
    uint64_t count = 0UL;
    for (auto &in : v) {
      auto sq = 0L;
      for (auto i = 0UL; auto c : in.values) {
        sq += shapes[i].first * shapes[i].second * c;
        ++i;
      }
      if (sq <= in.height * in.width) {
        ++count;
      }
    }
    return AOC_Output{count};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) { return AOC_Output{}; }
};

AOC_DAY_SOLUTION(AOCDaySolution);
