####
# CMakeLists.txt: Basic Setup
####
cmake_minimum_required(VERSION 3.13)
project(LedBlinkerBase C CXX)

# Include the build for F prime.
include("${CMAKE_CURRENT_LIST_DIR}/fprime/cmake/FPrime.cmake")
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime-Code.cmake")

# This includes project-wide objects
include("${CMAKE_CURRENT_LIST_DIR}/project.cmake")