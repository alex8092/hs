cmake_minimum_required(VERSION 2.8)

project(hs)

SET(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
SET(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(
  include/
)

file(
  GLOB_RECURSE
  source_files
  src/*
)

file(
  GLOB_RECURSE
  header_files
  include/*
)

add_executable(hs
  ${header_files}
  ${source_files}
)
