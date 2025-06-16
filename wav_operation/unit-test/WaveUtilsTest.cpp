#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/WaveUtils.hpp"

#include <limits>

namespace wave {


/*checkWaveParameters test*/
struct WaveParamTestCase {
    uint32_t num_channels;
    int sample_rate;
    uint32_t bytes_per_sample;
    uint32_t num_samples;
    bool expected;
};

class CheckWaveParametersTest : public ::testing::TestWithParam<WaveParamTestCase> {};

TEST_P(CheckWaveParametersTest, HandlesVariousInputs) {
    const auto& p = GetParam();
    EXPECT_EQ(checkWaveParameters(p.num_channels,
                               p.sample_rate,
                               p.bytes_per_sample,
                               p.num_samples),
                               p.expected);
}

INSTANTIATE_TEST_SUITE_P(
    WaveParameterTests,
    CheckWaveParametersTest,
    ::testing::Values(
        WaveParamTestCase{2, 44100, 2, 88200, true},  // valid
        WaveParamTestCase{0, 44100, 2, 88200, false}, // zero channels
        WaveParamTestCase{2, -1, 2, 88200, false},    // negative sample rate
        WaveParamTestCase{2, 44100, 0, 88200, false}, // zero bytes per sample
        WaveParamTestCase{2, static_cast<int>(std::numeric_limits<uint32_t>::max()) + 1LL, 2, 88200, false},
        WaveParamTestCase{static_cast<size_t>(std::numeric_limits<uint16_t>::max()) + 1, 44100, 2, 88200, false},
        WaveParamTestCase{2, 44100, 8192, 88200, false}, // bytes/sample * 8 too big
        WaveParamTestCase{2, 1'000'000'000, 4, 1'000'000, false}, // overflow in data rate
        WaveParamTestCase{2, 44100, 2, 88201, false} // samples not divisible by channels
    )
);

/*writeWaveHeader test*/
TEST(WaveHeaderTest, writeWaveHeader) {

}

/*readWaveHeader test*/
TEST(WaveheaderTest, readWaveHeader) {

}


} // namespace wave
