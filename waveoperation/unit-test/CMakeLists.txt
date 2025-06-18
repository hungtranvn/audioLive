set(TARGET test_endianness)

find_package(GTest REQUIRED)
find_library(GMock NAMES libgmock libgmock.a)

target_sources(
  ${TARGET}
  PRIVATE EndiannessTest.cpp
)

target_link_libraries(
  ${TARGET}
  GTEST::GTEST
  GTEST::Main
  ${GMock}
)
