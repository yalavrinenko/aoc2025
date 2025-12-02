//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include "fmt/ranges.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <ranges>
#include <string>
#include <string_view>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  uint64_t from;
  uint64_t to;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    return lines.front() | vw::split(',') | vw::transform([](auto s) {
             const auto sv = std::string_view(s);
             AOC_Input in;
             std::sscanf(sv.data(), "%ld-%ld", &in.from, &in.to);
             return in;
           }) |
           as_vector;
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  bool check(uint64_t value) {
    const auto casted = std::to_string(value);
    if (casted.size() % 2 != 0) {
      return false;
    }
    const auto l = as_value<uint64_t>(casted.substr(0, casted.size() / 2));
    const auto r = as_value<uint64_t>(casted.substr(casted.size() / 2));
    return l == r;
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto count = 0ULL;
    for (auto &in : v) {
      auto view = vw::iota(in.from, in.to + 1) |
                  vw::filter([&](auto val) { return check(val); });

      std::ranges::for_each(view, [&count](auto e) { count += e; });
    }
    return AOC_Output{count};
  }

  bool pattern_match(const auto &str, const auto &substr) {
    for (auto i = 0UL; i < str.size(); i += substr.size()) {
      if (substr != str.substr(i, substr.size())) {
        return false;
      }
    }
    return true;
  }

  bool complex_check(uint64_t value) {
    const auto casted = std::to_string(value);
    for (auto substr_size = 1; substr_size <= casted.size() / 2;
         ++substr_size) {
      if (casted.size() % substr_size != 0) {
        continue;
      }

      if (pattern_match(casted, casted.substr(0, substr_size))) {
        return true;
      }
    }
    return false;
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    auto count = 0ULL;
    for (auto &in : v) {
      auto view = vw::iota(in.from, in.to + 1) |
                  vw::filter([&](auto val) { return complex_check(val); });

      std::ranges::for_each(view, [&count](auto e) { count += e; });
    }
    return AOC_Output{count};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
