//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <cstdint>
#include <cstdio>
#include <fmt/color.h>
#include <fmt/core.h>

#include <fmt/ranges.h>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <vector>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  uint64_t target;
  std::vector<uint64_t> buttons;
};

struct AOC_Reader {

  static uint64_t get_target_bitmap(const std::string &line) {
    char target_str[255];
    std::sscanf(line.c_str(), "[%s]", target_str);

    uint64_t target = 0;

    auto pos = 0;
    char *c = target_str;
    while (*c != '\0' && *c != ']') {
      if (*c == '#') {
        target |= 1 << pos;
      }
      ++pos;
      ++c;
    }
    return target;
  }

  static std::vector<uint64_t> get_button_bitmaps(const std::string &line) {
    std::vector<uint64_t> bitmaps;
    std::regex rexpr("\\([\\d+,]*\\)");
    std::vector groups(std::sregex_iterator{line.begin(), line.end(), rexpr},
                       std::sregex_iterator{});

    bitmaps = groups | vw::transform([](auto &group) {
                uint64_t map = 0UL;
                auto str = group.str();
                std::regex rexpr{"(\\d+)"};
                auto positions =
                    std::ranges::subrange{
                        std::sregex_iterator{str.begin(), str.end(), rexpr},
                        std::sregex_iterator{}} |
                    vw::transform([](auto &sg) { return std::stol(sg.str()); });
                for (auto p : positions) {
                  map |= 1 << p;
                }
                return map;
              }) |
              as_vector;

    return bitmaps;
  }

  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    return lines | vw::transform([](auto &line) {
             return AOC_Input{
                 .target = get_target_bitmap(line),
                 .buttons = get_button_bitmaps(line),
             };
           }) |
           as_vector;
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  uint64_t bfs(const AOC_Input &in) {
    struct Node {
      uint64_t value;
      uint64_t depth;
    };
    std::queue<Node> Q;
    Q.emplace(0, 0);

    std::set<uint64_t> visited{Q.front().value};

    while (!Q.empty()) {
      auto front = Q.front();
      Q.pop();

      for (auto &sw : in.buttons) {
        auto next_state = front.value ^ sw;
        if (next_state == in.target) {
          return front.depth + 1;
        }

        if (!visited.contains(next_state)) {
          Q.emplace(next_state, front.depth + 1);
        }
      }
    }

    throw "Fuck";
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    auto sum = 0UL;
    for (auto &in : v) {
      sum += bfs(in);
    }
    return AOC_Output{sum};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) { return AOC_Output{}; }
};

AOC_DAY_SOLUTION(AOCDaySolution);
