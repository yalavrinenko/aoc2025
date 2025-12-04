//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include "fmt/ranges.h"

#include <fmt/color.h>
#include <fmt/core.h>

#include <array>
#include <ranges>
#include <string>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Reader {
  static std::vector<std::string>
  from_string_vector(const std::vector<std::string> &lines) {
    return lines;
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, std::string, AOC_Reader> {
  friend AOCSolution;

private:
  int count_adjacent_at_symbols(const std::vector<std::string> &grid, int row,
                                int col) const {
    int count = 0;

    // Directions for 8 neighboring cells (dx, dy)
    const std::array<std::pair<int, int>, 8> directions = {{
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, -1},
        {0, 1},
        {1, -1},
        {1, 0},
        {1, 1},
    }};

    for (const auto &[dx, dy] : directions) {
      int new_row = row + dx;
      int new_col = col + dy;

      // Check boundaries
      if (new_row >= 0 && new_row < static_cast<int>(grid.size()) &&
          new_col >= 0 && new_col < static_cast<int>(grid[new_row].size())) {
        if (grid[new_row][new_col] == '@') {
          count++;
        }
      }
    }

    return count;
  }

public:
  AOC_Output part1(std::vector<std::string> const &v) {
    auto counter = 0UL;
    for (auto i : vw::iota(0UL, v.size())) {
      for (auto j : vw::iota(0UL, v.front().size())) {
        if (v[i][j] == '@' && count_adjacent_at_symbols(v, i, j) < 4) {
          ++counter;
        }
      }
    }
    return AOC_Output{counter};
  }

  AOC_Output part2(std::vector<std::string> const &v) {
    auto counter = 0UL;
    auto grid = v;
    auto removed = 0;
    do {
      auto next_grid = grid;

      removed = 0;
      for (auto i : vw::iota(0UL, grid.size())) {
        for (auto j : vw::iota(0UL, grid.front().size())) {
          if (grid[i][j] == '@' && count_adjacent_at_symbols(grid, i, j) < 4) {
            ++counter;
            ++removed;

            next_grid[i][j] = '.';
          }
        }
      }

      grid = next_grid;
    } while (removed > 0);
    return AOC_Output{counter};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
