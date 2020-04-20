cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ---- Project ----

project(BitLens 
  VERSION 2.2
  LANGUAGES CXX
)

# ---- Include guards ----

if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# ---- Dependencies ----

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME PackageProject.cmake
  GITHUB_REPOSITORY TheLartians/PackageProject.cmake
  VERSION 1.1
)

# ---- Make headers visible in IDEs ----

FILE(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_library(BitLensHeaders EXCLUDE_FROM_ALL ${headers})
SET_TARGET_PROPERTIES(BitLensHeaders PROPERTIES LINKER_LANGUAGE CXX)

# ---- Create library ----

add_library(BitLens INTERFACE)
set_target_properties(BitLens PROPERTIES INTERFACE_COMPILE_FEATURES cxx_std_17)

target_include_directories(BitLens
  INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/BitLensConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# ---- Install ----

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  DEPENDENCIES "EasyIterator;BitLens"
)
