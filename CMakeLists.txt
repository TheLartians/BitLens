cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ---- Project ----

project(BitLens 
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

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
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/BitLensConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# ---- Install ----

install(
  TARGETS BitLens
  EXPORT BitLensTargets
  LIBRARY DESTINATION lib COMPONENT Runtime
  ARCHIVE DESTINATION lib COMPONENT Development
  RUNTIME DESTINATION bin COMPONENT Runtime
  PUBLIC_HEADER DESTINATION include COMPONENT Development
  BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/BitLensConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/BitLensConfig.cmake"
  INSTALL_DESTINATION lib/cmake/BitLens
)

install(
  EXPORT BitLensTargets 
  DESTINATION lib/cmake/BitLens
)

install(
  FILES 
    "${PROJECT_BINARY_DIR}/BitLensConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}/BitLensConfig.cmake"
  DESTINATION 
    lib/cmake/BitLens
)

install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION include
)
