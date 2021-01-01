cmake_minimum_required(VERSION 3.11)
project(Multifarious CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(multifarious INTERFACE)
target_include_directories(multifarious INTERFACE include)
