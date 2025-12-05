//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <algorithm>
#include <cstdint>
#include <fmt/color.h>
#include <fmt/core.h>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOCInput {
  struct InclusiveRange {
    uint64_t begin, end;
  };
  std::vector<InclusiveRange> ranges;
  std::vector<uint64_t> ids;
};

struct AOC_Reader {
  static std::vector<AOCInput>
  from_string_vector(const std::vector<std::string> &lines) {
    AOCInput result;
    bool parsingRanges = true;
    for (auto &line : lines) {
      if (line.empty()) {
        parsingRanges = false;
        continue;
      }

      if (parsingRanges) {
        uint64_t begin, end;
        std::sscanf(line.c_str(), "%lu-%lu", &begin, &end);
        result.ranges.emplace_back(AOCInput::InclusiveRange{begin, end});
      } else {
        result.ids.emplace_back(as_value<uint64_t>(line));
      }
    }
    return {result};
  }
};

class AOCDaySolution
    : public AOCSolution<AOCDaySolution, AOCInput, AOC_Reader> {
  friend AOCSolution;

  AOC_Output part1(std::vector<AOCInput> const &v) {
    const auto &in = v.front();

    auto check_fresh = [&in](auto id) {
      return std::ranges::any_of(
          in.ranges, [id](auto r) { return r.begin <= id && id <= r.end; });
    };

    const uint64_t fresh = std::ranges::count_if(in.ids, check_fresh);

    return AOC_Output{fresh};
  }

  std::optional<AOCInput::InclusiveRange> merge(AOCInput::InclusiveRange lhs,
                                                AOCInput::InclusiveRange rhs) {
    if (lhs.end < rhs.begin) {
      return {};
    }

    AOCInput::InclusiveRange result{.begin = std::min(lhs.begin, rhs.begin),
                                    .end = std::max(lhs.end, rhs.end)};
    return result;
  }

  AOC_Output part2(std::vector<AOCInput> const &v) {
    auto segments = v.front().ranges;

    std::ranges::sort(segments, {}, &AOCInput::InclusiveRange::begin);

    std::vector<AOCInput::InclusiveRange> merged_segments;

    auto current_segment = segments.front();

    for (auto s : segments) {
      const auto merged = merge(current_segment, s);
      if (merged) {
        current_segment = *merged;
      } else {
        merged_segments.emplace_back(current_segment);
        current_segment = s;
      }
    }
    merged_segments.emplace_back(current_segment);

    const auto count = std::accumulate(
        merged_segments.begin(), merged_segments.end(), 0ULL,
        [](auto sum, auto s) { return sum + 1 + (s.end - s.begin); });

    return AOC_Output{count};
  }
};

AOC_DAY_SOLUTION(AOCDaySolution);
