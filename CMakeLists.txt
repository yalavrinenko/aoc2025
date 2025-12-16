cmake_minimum_required(VERSION 3.19)
project(aoc2023)

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 123913715afeb8a437e6388b4473fcc4753e1c9a # v11.1.4
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(fmt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

function(aoc_day_solution day_id)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/solution.cpp")

  else()
    file(MAKE_DIRECTORY ${day_id})
    file(TOUCH ${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/data.in)
    configure_file(template.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/solution.cpp COPYONLY)
  endif()
  add_executable(${day_id} ${CMAKE_CURRENT_SOURCE_DIR}/${day_id}/solution.cpp)
  target_compile_definitions(${day_id}
                             PUBLIC -DDAY_PATH=\"../${day_id}/data.in\")
  target_link_libraries(${day_id} fmt::fmt-header-only -lpthread)
endfunction()

aoc_day_solution(1)
aoc_day_solution(2)
aoc_day_solution(3)
aoc_day_solution(4)
aoc_day_solution(5)
aoc_day_solution(6)
aoc_day_solution(7)
aoc_day_solution(8)
aoc_day_solution(9)
aoc_day_solution(10)
aoc_day_solution(11)
aoc_day_solution(12)
