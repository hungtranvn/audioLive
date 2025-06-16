cmake_minimum_required(VERSION 3.5)

project(
    RealtimeService
    LANGUAGES CXX
)

find_package(log4cplus 2.0.0 REQUIRED)

include_directories(${LOG4CPLUS_INCLUDE_DIRS})
link_directories(${LOG4CPLUS_LIBRARY_DIRS})


add_compile_options(
    -Wall
    -Wextra
    -Werror=array-bounds
    -Werror=char-subscripts
    -Werror=nonnull
    -Werror=parentheses
    -Werror=return-type
    -Werror=sequence-point
    -Werror=sign-compare
    -Werror=switch
    -Werror=uninitialized
    -Werror=empty-body
    -Werror=missing-field-initializers
    -Werror=type-limits
    -Werror=unused-parameter
    -Werror=format-security
    -Werror=switch-enum
    -s
    -Werror=return-type
    -Wno-ignored-qualifiers
    -Wno-unused-parameter
    -Wformat
    -D_FORTIFY_SOURCE=2
    -D_GLIBCCXX_ASERTIOND
    -O
    -fstack-protector-strong
    -fPIC
    -fpic
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_UNIT_TESTS "Enables the unit tests." ON)

add_subdirectory(src)

