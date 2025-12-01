//
// Created by yalavrinenko on 02.12.2020.
//

#ifndef AOC_2020_MAIN_HPP
#define AOC_2020_MAIN_HPP
#include <vector>

#include <chrono>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <fstream>
#include <ranges>

namespace vw = std::ranges::views;

template <typename Base, typename AOC_Input, typename AOC_Reader>
class AOCSolution {
public:
  std::vector<AOC_Input> AOC_ParceInput(std::string const &path) {
    std::ifstream in(path);
    std::string line;
    std::vector<std::string> v;
    while (std::getline(in, line))
      v.emplace_back(line);
    return AOC_Reader::from_string_vector(v);
  }

  template <typename func_t, typename... args_t>
  void execute_with_timer(std::string_view message, func_t func,
                          args_t... args) {
    auto stime = std::chrono::high_resolution_clock::now();
    auto result = func(std::forward<args_t...>(args...));
    auto ftime = std::chrono::high_resolution_clock::now();
    fmt::print(fmt::emphasis::bold | fg(fmt::color::green), "{}: {}\n", message,
               result());
    fmt::print(
        fmt::emphasis::bold | fg(fmt::color::red), "Working time {} μs\n\n",
        std::chrono::duration_cast<std::chrono::microseconds>(ftime - stime)
            .count());
  }

  void solve() {
    execute_with_timer(
        "Part 1",
        [this](auto &&v) { return self().part1(std::forward<decltype(v)>(v)); },
        AOC_ParceInput(DAY_PATH));
    execute_with_timer(
        "Part 2",
        [this](auto &&v) { return self().part2(std::forward<decltype(v)>(v)); },
        AOC_ParceInput(DAY_PATH));
  }

private:
  Base &self() { return *static_cast<Base *>(this); }
};

#define AOC_DAY_SOLUTION(SOLUTION_CLASS)                                       \
  int main() {                                                                 \
    SOLUTION_CLASS solution;                                                   \
    solution.solve();                                                          \
    return 0;                                                                  \
  }

template<typename V>
auto to_vector(V &&v) {
  decltype(auto) cv = std::forward<V>(v) | vw::common;
  return std::vector(cv.begin(), cv.end());
}

struct as_vector_impl {};

inline auto as_vector = as_vector_impl{};

template<typename V>
inline auto operator|(V &&v, const as_vector_impl &obj) {
  return to_vector(std::forward<V>(v));
}

template<typename T>
struct srange_to_value {
  static auto operator()(auto &&r) {
    T init{};
    for (auto e: r) {
      init = init * 10 + e - '0';
    }
    return init;
  }
};

template <typename T> auto as_value(std::string_view s) {
  T value{};
  if (std::from_chars(s.begin(), s.end(), value).ec == std::errc{}) {
    return value;
  } else {
    throw std::runtime_error{"Fail to parse int."};
  }
}

#endif//AOC_2020_MAIN_HPP
