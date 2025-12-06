//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::vector<uint64_t>> data;
  std::vector<char> op;

  std::vector<std::string> raw;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    AOC_Input in;
    for (auto const &line : lines | vw::take(lines.size() - 1)) {
      auto cleared_line = line;
      auto it =
          std::unique(cleared_line.begin(), cleared_line.end(),
                      [](char a, char b) { return a == ' ' && b == ' '; });
      cleared_line.erase(it, cleared_line.end());
      while (cleared_line.back() == ' ') {
        cleared_line.pop_back();
      }
      while (cleared_line.front() == ' ') {
        cleared_line.erase(0, 1);
      }

      in.data.emplace_back(cleared_line | vw::split(' ') |
                           vw::transform([](auto s) {
                             const std::string_view sv{s};
                             return as_value<uint64_t>(sv);
                           }) |
                           as_vector);
    }

    auto cleared_line = lines.back();
    auto it = std::unique(cleared_line.begin(), cleared_line.end(),
                          [](char a, char b) { return a == ' ' && b == ' '; });
    cleared_line.erase(it, cleared_line.end());
    while (cleared_line.back() == ' ') {
      cleared_line.pop_back();
    }

    in.op = cleared_line | vw::split(' ') |
            vw::transform([](auto s) { return s[0]; }) | as_vector;
    in.raw = lines;
    return {in};
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    const auto in = v.front();

    auto sum = 0ULL;
    for (auto column = 0UL; column < in.op.size(); ++column) {

      auto data =
          in.data | vw::transform([column](auto &row) { return row[column]; });

      auto column_res =
          std::accumulate(data.begin() + 1, data.end(), data.front(),
                          [op = in.op[column]](auto sum, auto v) {
                            return (op == '+') ? sum + v : sum * v;
                          });
      sum += column_res;
    }

    return AOC_Output{sum};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    auto in = v.front();
    in.data.clear();

    for (auto &row : in.raw) {
      row += " ";
    }

    auto column_sep =
        vw::iota(0UL, in.raw.front().size()) | vw::filter([&in](auto id) {
          auto data =
              in.raw | vw::transform([id](auto &row) { return row[id]; });
          return std::ranges::all_of(data, [](auto c) { return c == ' '; });
        });

    auto begin = 0UL;
    for (auto end : column_sep) {
      in.data.emplace_back();
      for (auto i : vw::iota(begin, end)) {
        auto value_v = in.raw |
                       vw::transform([i](auto &row) { return row[i]; }) |
                       vw::filter([](auto c) { return c != ' '; });
        in.data.back().emplace_back(
            as_value<uint64_t>(fmt::format("{}", fmt::join(value_v, ""))));
      }
      begin = end + 1;
    }

    auto sum = 0ULL;
    for (auto cid = 0; auto data : in.data) {

      auto column_res =
          std::accumulate(data.begin() + 1, data.end(), data.front(),
                          [op = in.op[cid]](auto sum, auto v) {
                            return (op == '+') ? sum + v : sum * v;
                          });
      ++cid;
      sum += column_res;
    }

    return AOC_Output{sum};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
