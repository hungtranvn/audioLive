cmake_minimum_required(VERSION 3.20)
project(scalar)
add_library(scalar SHARED
    include/core/base.hpp
    include/core/defs.hpp
    include/core/matx.hpp
)