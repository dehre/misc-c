cmake_minimum_required(VERSION 3.21)

project(misc-c VERSION 1.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_CLANG_TIDY clang-tidy)

add_compile_options(
  -Wall
  -Wextra
  -Wcast-qual
  -Wconversion
  -Wno-sign-conversion
  -g3
  -O0
  $<$<CONFIG:RELEASE>:-g0> # overwrites -g3
  $<$<CONFIG:RELEASE>:-O3> # overwrites -O0
  $<$<CONFIG:ASAN>:-fsanitize=address>
  $<$<CONFIG:ASAN>:-fno-omit-frame-pointer>
  $<$<CONFIG:UBSAN>:-fsanitize=undefined>
  $<$<CONFIG:UBSAN>:-fno-omit-frame-pointer>
  $<$<CONFIG:TSAN>:-fsanitize=thread>)

add_link_options(
  -pthread $<$<CONFIG:ASAN>:-fsanitize=address>
  $<$<CONFIG:UBSAN>:-fsanitize=undefined> $<$<CONFIG:TSAN>:-fsanitize=thread>)

# Add `DEBUG` symbol when not in release mode
add_compile_definitions($<$<NOT:$<CONFIG:RELEASE>>:DEBUG>)

# 
# Moore Finite State Machine
# 
add_executable(fst src/finite-state-machine/fst.c)
# target_link_libraries(fst PUBLIC somelib)

# 
# Multi-Producer Multi-Consumer FIFO Queue
# 
add_executable(fifo-mpmc src/fifo-multi-producer-multi-consumer/fifo-mpmc.c)
