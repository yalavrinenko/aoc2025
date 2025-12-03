//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include "fmt/ranges.h"

#include <algorithm>
#include <cstdint>
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

struct AOC_Input {
  std::vector<int> bank;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    return lines | vw::transform([](auto line) {
             return AOC_Input{
                 .bank = line | vw::transform([](auto c) { return c - '0'; }) |
                         as_vector};
           }) |
           as_vector;
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  int find_largest(const std::vector<int> &bank) {
    auto max = 0;
    for (auto i = 0UL; i < bank.size() - 1; ++i) {
      for (auto j = i + 1; j < bank.size(); ++j) {
        if (bank[i] * 10 + bank[j] > max) {
          max = bank[i] * 10 + bank[j];
        }
      }
    }
    return max;
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto sum = 0UL;
    for (auto &in : v) {
      sum += find_largest(in.bank);
    }
    return AOC_Output{sum};
  }

  uint64_t to_number(const std::vector<int> &digits, int ignore) {
    uint64_t res = 0;
    for (auto i = 0; auto d : digits) {
      if (i != ignore) {
        res = res * 10 + d;
      }
      ++i;
    }
    return res;
  }

  uint64_t find_larges_v2(std::vector<int> const &bank) {
    static constexpr auto Size = 12U;

    std::vector<int> largest(bank.begin(), bank.begin() + Size);
    uint64_t largest_v = to_number(largest, Size);

    for (auto d : bank | vw::drop(Size)) {
      largest.emplace_back(d);

      auto ignore_for_max = Size;
      for (auto i : vw::iota(0U, Size)) {
        const auto value = to_number(largest, i);
        if (value > largest_v) {
          largest_v = value;
          ignore_for_max = i;
        }
      }

      largest.erase(largest.begin() + ignore_for_max);
    }
    return largest_v;
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    auto sum = 0ULL;
    for (auto &in : v) {
      const auto largest = find_larges_v2(in.bank);
      sum += largest;
    }
    return AOC_Output{sum};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
