cmake_minimum_required(VERSION 3.22)

project(smooth_operator VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(CTest)
find_package(GTest REQUIRED)

add_subdirectory(lib)