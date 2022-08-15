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

# # # # # # # # # # # # # # # # # # # # # 
# LIBRARIES
# # # # # # # # # # # # # # # # # # # # # 

add_library(common_semaphores libs/common_semaphores/macos.c)
target_include_directories(common_semaphores PUBLIC ${PROJECT_SOURCE_DIR}/src/include)

# # # # # # # # # # # # # # # # # # # # # 
# EXECUTABLES
# # # # # # # # # # # # # # # # # # # # # 

add_executable(fst src/finite-state-machine/fst.c)

add_executable(fifo-mpmc src/fifo-multi-producer-multi-consumer/fifo-mpmc.c)
target_include_directories(fifo-mpmc PRIVATE src/include)
target_link_libraries(fifo-mpmc PUBLIC common_semaphores)

add_executable(fifo-spmc src/fifo-single-producer-multi-consumer/fifo-spmc.c)
target_include_directories(fifo-spmc PRIVATE src/include)
target_link_libraries(fifo-spmc PUBLIC common_semaphores)

add_executable(fifo-spsc src/fifo-single-producer-single-consumer/fifo-spsc.c)
target_include_directories(fifo-spsc PRIVATE src/include)
target_link_libraries(fifo-spsc PUBLIC common_semaphores)
