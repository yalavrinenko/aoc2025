//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include "fmt/ranges.h"

#include <cstdint>
#include <fmt/color.h>
#include <fmt/core.h>

#include <iterator>
#include <map>
#include <queue>
#include <set>
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
  std::map<std::string, std::vector<std::string>> adj;
};

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &lines) {
    AOC_Input in;
    for (const auto &line : lines) {
      std::istringstream iss{line};
      std::string name;
      iss >> name;
      name.pop_back();

      in.adj[name] = std::vector(std::istream_iterator<std::string>{iss},
                                 std::istream_iterator<std::string>{});
    }
    return {in};
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOC_Input, AOC_Reader> {
  friend AOCSolution;

  uint64_t
  path_counter_v2(const std::string &src, const std::string &dst,
                  std::map<std::string, std::vector<std::string>> const &adj) {
    std::map<std::string, uint64_t> pathes;
    pathes[src] = 1;

    std::queue<std::string> Q;
    Q.emplace(src);

    std::set<std::string> visited;

    while (!Q.empty()) {
      auto front = Q.front();
      Q.pop();

      if (front == dst) {
        return pathes[front];
      }

      if (!visited.contains(front)) {
        visited.emplace(front);

        if (adj.contains(front)) {
          for (const auto &next : adj.at(front)) {
            pathes[next] += pathes[front];
            Q.emplace(next);
          }
        }
      }
    }

    return 0;
  }

  AOC_Output part1(std::vector<AOC_Input> const &v) {
    uint64_t pathes = 0;

    try {
      pathes = path_counter_v2("you", "out", v.front().adj);
    } catch (...) {
    }

    return AOC_Output{pathes};
  }

  AOC_Output part2(std::vector<AOC_Input> const &v) {
    auto srv_to_fft = path_counter_v2("svr", "fft", v.front().adj);
    auto fft_to_dac = path_counter_v2("fft", "dac", v.front().adj);
    auto dac_to_out = path_counter_v2("dac", "out", v.front().adj);

    fmt::print("{} {} {}\n", srv_to_fft, fft_to_dac, dac_to_out);

    return AOC_Output{srv_to_fft * fft_to_dac * dac_to_out};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
