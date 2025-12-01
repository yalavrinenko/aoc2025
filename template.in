//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <charconv>
#include <fmt/color.h>
#include <fmt/core.h>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string line;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    return {};
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    return AOC_Output{};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) { return AOC_Output{}; }
};

AOC_DAY_SOLUTION(AOCDaySolution);
