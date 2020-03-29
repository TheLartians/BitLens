cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(BitViewTests
  LANGUAGES CXX
)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME doctest
  GITHUB_REPOSITORY onqtam/doctest
  VERSION 2.3.2
  GIT_TAG 2.3.2
)

CPMAddPackage(
  NAME BitView
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
)

CPMAddPackage(
  NAME Format.cmake
  GITHUB_REPOSITORY TheLartians/Format.cmake
  VERSION 1.0
)

# ---- Create binary ----

file(GLOB BitViewTests_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(BitViewTests ${BitViewTests_sources})
target_link_libraries(BitViewTests doctest BitView)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set_target_properties(BitViewTests PROPERTIES CXX_STANDARD 17 COMPILE_FLAGS "-Wall -pedantic -Wextra -Werror")
else()
  set_target_properties(BitViewTests PROPERTIES CXX_STANDARD 17)
endif()

# ---- Add BitViewTests ----

ENABLE_TESTING() 
ADD_TEST(BitViewTests BitViewTests)

# ---- code coverage ----

if (${ENABLE_TEST_COVERAGE})
  set_target_properties(BitViewTests PROPERTIES CXX_STANDARD 17 COMPILE_FLAGS "-O0 -g -fprofile-arcs -ftest-coverage --coverage")
  target_link_options(BitViewTests PUBLIC "--coverage")
endif()
