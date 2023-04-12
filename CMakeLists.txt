# Require the CMake version matching what is included in the latest LTS version
# of Ubuntu, which at the time of writing is 22.04.
cmake_minimum_required(VERSION 3.22)

# TODO: Define a good project in the line below. If applicable, list other
# languages in this project managed by CMake.
project(ProjectTitle
  VERSION 1.0
  LANGUAGES C
)

# Use the latest C standard supported by Clang and LLVM in the latest LTS
# version of Ubuntu.
set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "
  -Wall \
  -Wextra \
  -pedantic \
  -Wconversion \
  -Wdouble-promotion \
  -Wduplicated-branches \
  -Wduplicated-cond \
  -Wimplicit-fallthrough \
  -Wlogical-op \
  -Wmisleading-indentation \
  -Wnull-dereference \
  -Wshadow \
  -Wsign-conversion \
  -Wunreachable-code \
  -Wunused \
  -Wuseless-cast \
  -g \
")

# No options are set here because the configuration is set in .clang-tidy
# instead. If that file is not present, clang-tidy will be of limited help.
set(CMAKE_C_CLANG_TIDY "clang-tidy")

# Include the source files.
add_subdirectory(src)

# In current courses that teach C, we use the Criterion testing framework. If
# If you want to use a different framework, or if you want to skip testing
# entirely (not recommended), change the lines below.
include(CTest)
find_library(CRITERION criterion REQUIRED)
set(CRITERION_FLAGS --verbose -j1)

add_subdirectory(test)
