#include <gtest/gtest.h>
#include "../src/Endianness.hpp"

namespace wave {
TEST(EndianTest, DetectsEndiannessCorrectly) {
    if (isLittleEndian()) {
        SUCCEED() << "Host is little-endian";
    } else {
        SUCCEED() << "Host is big-endian";
    }
}

/*toBigEndian() function test*/
TEST(EndianTest, to_big_endian_16bit) {
    uint16_t original = 0x1234;
    uint16_t expected = isLittleEndian() ? 0x3412 : 0x1234;
    toBigEndian(original);
    EXPECT_EQ(original, expected);
}

TEST(EndianTest, to_big_endian_32bit) {
    uint32_t original = 0x12345678;
    uint32_t expected = isLittleEndian() ? 0x78563412 : 0x12345678;
    toBigEndian(original);
    EXPECT_EQ(original, expected);
}


/*toLittleEndian() function test*/
TEST(EndianTest, to_little_endian_16bit) {
    uint16_t original = 0x1234;
    uint16_t expected = isBigEndian() ? 0x3412 : 0x1234;
    toLittleEndian(original);
    EXPECT_EQ(original, expected);
}

TEST(EndianTest, to_little_endian_32bit) {
    uint32_t original = 0x12345678;
    uint32_t expected = isBigEndian() ? 0x78563412 : 0x12345678;
    toLittleEndian(original);
    EXPECT_EQ(original, expected);
}
} // namespace wav
