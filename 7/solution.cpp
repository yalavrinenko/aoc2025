//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include "fmt/ranges.h"

#include <algorithm>
#include <cstdint>
#include <fmt/color.h>
#include <fmt/core.h>
#include <iterator>
#include <numeric>
#include <queue>
#include <string>

#include <vector>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct Map {
  std::vector<std::string> map;
  uint64_t split_count;
};

struct AOC_Reader {
  static std::vector<Map>
  from_string_vector(const std::vector<std::string> &lines) {
    Map m;
    m.split_count = 0;
    m.map = lines;
    return {m};
  }
};

class Ray {
public:
  Ray(uint64_t x, uint64_t y, Map &map) : x{x}, y{y}, map_ptr_(&map) {
    if (x == map.map.size() - 1) {
      is_stop_ = true;
    }
  }

  void move(std::vector<Ray> &rays) {
    auto &map = *map_ptr_;
    if (map.map[x + 1][y] == '.') {
      x += 1;
      map.map[x][y] = '|';
      if (x == map.map.size() - 1) {
        is_stop_ = true;
      }

      rays.emplace_back(*this);
    } else {
      if (map.map[x + 1][y] == '^') {
        x += 1;
        ++map.split_count;

        if (y - 1 >= 0 && map.map[x][y - 1] == '.') {
          map.map[x][y - 1] = '|';
          rays.emplace_back(x, y - 1, map);
        }

        if (y + 1 < map.map.front().size() && map.map[x][y + 1] == '.') {
          map.map[x][y + 1] = '|';
          rays.emplace_back(x, y + 1, map);
        }
      }
    }
  }

  bool is_active() const { return !is_stop_; }

  bool operator==(const Ray &rhs) const { return x == rhs.x && y == rhs.y; }

private:
  uint64_t x;
  uint64_t y;

  bool is_stop_ = false;

  Map *map_ptr_;
};

class AOCDaySolution : public AOCSolution<AOCDaySolution, Map, AOC_Reader> {
  friend AOCSolution;

  AOC_Output part1(std::vector<Map> const &v) {

    auto m = v.front();

    auto spos = std::ranges::find(m.map.front(), 'S');
    std::vector<Ray> rays{
        Ray(0, std::distance(m.map.front().begin(), spos), m)};

    while (std::ranges::all_of(rays, &Ray::is_active)) {
      std::vector<Ray> next_rays;
      for (auto &r : rays) {
        if (r.is_active()) {
          r.move(next_rays);
        }
      }

      rays = next_rays;
    }

    return AOC_Output{m.split_count};
  }

  AOC_Output part2(std::vector<Map> const &v) {
    auto m = v.front();

    auto spos = std::ranges::find(m.map.front(), 'S');
    std::vector<Ray> rays{
        Ray(0, std::distance(m.map.front().begin(), spos), m)};

    while (std::ranges::all_of(rays, &Ray::is_active)) {
      std::vector<Ray> next_rays;
      for (auto &r : rays) {
        if (r.is_active()) {
          r.move(next_rays);
        }
      }

      rays = next_rays;
    }

    std::vector<std::vector<uint64_t>> D(
        m.map.size(), std::vector<uint64_t>(m.map.front().size(), 0));

    struct Point {
      uint64_t x, y;
    };

    std::queue<Point> Q;
    auto s = Point{
        .x = 0,
        .y = static_cast<uint64_t>(std::distance(m.map.front().begin(), spos)),
    };
    Q.emplace(s);
    D[s.x][s.y] = 1;

    while (!Q.empty()) {
      auto c = Q.front();
      Q.pop();

      if (c.x == D.size() - 1) {
        continue;
      }

      auto nx = c.x + 1;
      if (m.map[nx][c.y] == '|') {
        Point np{nx, c.y};
        if (D[np.x][np.y] == 0) {
          Q.emplace(np);
        }
        D[np.x][np.y] += D[c.x][c.y];
      } else if (m.map[nx][c.y] == '^') {
        for (auto dy : {-1, 1}) {
          auto ny = c.y + dy;
          if (0 <= ny && ny < D.front().size()) {
            if (D[nx][ny] == 0) {
              Q.emplace(Point{.x = nx, .y = ny});
            }
            D[nx][ny] += D[c.x][c.y];
          }
        }
      }
    }

    auto opt = std::accumulate(D.back().begin(), D.back().end(), 0UL);

    return AOC_Output{opt};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
