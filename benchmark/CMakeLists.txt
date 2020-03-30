cmake_minimum_required (VERSION 3.14)

# ---- create project ----

project(BitViewBenchmark 
  LANGUAGES CXX
)

add_executable(BitViewBenchmark "benchmark.cpp")
set_target_properties(BitViewBenchmark PROPERTIES CXX_STANDARD 17)        

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME BitView
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
)

CPMAddPackage(
  NAME googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  VERSION 1.5.0
  OPTIONS
   "BENCHMARK_ENABLE_TESTING Off"
   "BENCHMARK_USE_LIBCXX ON"
)

# patch google benchmark target
set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)        
target_link_libraries(BitViewBenchmark benchmark BitView)
