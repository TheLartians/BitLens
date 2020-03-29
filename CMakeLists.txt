cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ---- Project ----

project(BitView 
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# ---- Make headers visible in IDEs ----

FILE(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_library(BitViewHeaders EXCLUDE_FROM_ALL ${headers})
SET_TARGET_PROPERTIES(BitViewHeaders PROPERTIES LINKER_LANGUAGE CXX)

# ---- Create library ----

add_library(BitView INTERFACE)
set_target_properties(BitView PROPERTIES INTERFACE_COMPILE_FEATURES cxx_std_17)

target_include_directories(BitView
  INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/BitViewConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# ---- Install ----

install(
  TARGETS BitView
  EXPORT BitViewTargets
  LIBRARY DESTINATION lib COMPONENT Runtime
  ARCHIVE DESTINATION lib COMPONENT Development
  RUNTIME DESTINATION bin COMPONENT Runtime
  PUBLIC_HEADER DESTINATION include COMPONENT Development
  BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/BitViewConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/BitViewConfig.cmake"
  INSTALL_DESTINATION lib/cmake/BitView
)

install(
  EXPORT BitViewTargets 
  DESTINATION lib/cmake/BitView
)

install(
  FILES 
    "${PROJECT_BINARY_DIR}/BitViewConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}/BitViewConfig.cmake"
  DESTINATION 
    lib/cmake/BitView
)

install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION include
)
