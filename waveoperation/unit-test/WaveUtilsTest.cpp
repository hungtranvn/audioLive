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

class ReadableWaveBuffer : public ReadableWave {
public:
    ReadableWaveBuffer(const uint8_t* buf, size_t size) 
        : m_buf(buf), m_size(size), m_pos(0), m_buf_exhausted(false), m_check_read_size(true) {}

    ReadableWaveBuffer(const uint8_t* buf, size_t size, bool check_read_size)
        : m_buf(buf), m_size(size), m_pos(0), m_check_read_size(check_read_size) {}

    virtual ~ReadableWaveBuffer() {
        if (m_check_read_size)
            EXPECT_EQ(m_size, m_pos);
    }

    virtual size_t read(void* buf, size_t num_bytes) {
        if (m_size >= WAVE_HEADER_SIZE)
            EXPECT_GE(m_size, m_pos + num_bytes);
        EXPECT_FALSE(m_buf_exhausted);

        const size_t bytes_remaining = m_size - m_pos;
        if (num_bytes > bytes_remaining) {
            m_buf_exhausted = true;
            num_bytes = bytes_remaining;
        }
        memcpy(buf, &m_buf[m_pos], num_bytes);
        m_pos += num_bytes;
        
        return num_bytes;
    }

private:
    const uint8_t* m_buf;
    const size_t m_size;
    size_t m_pos;
    bool m_buf_exhausted;
    const bool m_check_read_size;
};

/*writeWaveHeader test*/
/*
TEST(WaveHeaderTest, writeWaveHeader) {

}
*/

/*readWaveHeader test*/
TEST(WaveheaderTest, readWaveHeader) {
    size_t num_channels = 0;
    int sample_rate = 0;
    size_t bytes_per_sample = 0;
    size_t num_samples = 0;

    {
        static const uint8_t badRiffId[] = {
            'R', 'i', 'f', 'f', // bad
            0xbd, 0xd0, 0x5b, 0x07, // size_of_whole_file - 8 = 123457689 + 44 -8
            'W', 'A', 'V', 'E',
            'f', 'm', 't', ' ', // subchun1Id
            16, 0, 0, 0, // size_of_fmt - 8 = 24 - 8
            6, 0, // format = 6
            17, 0, // channels = 17
            0x39, 0x30, 0, 0, // sample_rate = 12345 
            0xc9, 0x33, 0x03, 0, // byte_rate = 1 * 17 * 12345
            17, 0, // block_align = num_channels*bytes_per_sample
            8, 0, // bits_per_sample
            'd', 'a', 't', 'a', // subchunk2Id
            0x99, 0xd0, 0x5b, 0x07, 
        };

        ReadableWaveBuffer r(badRiffId, sizeof(badRiffId));
        EXPECT_FALSE(readWaveHeader(&r, &num_channels, &sample_rate,
                       &bytes_per_sample, &num_samples));
    }

    {
        static const uint8_t badBitsPerSample[] = {
            'R', 'I', 'F', 'F', // 
            0xbd, 0xd0, 0x5b, 0x07, // size_of_whole_file - 8 = 123457689 + 44 -8
            'W', 'A', 'V', 'E',
            'f', 'm', 't', ' ', // subchun1Id
            16, 0, 0, 0, // size_of_fmt - 8 = 24 - 8
            6, 0, // format = 6
            17, 0, // channels = 17
            0x39, 0x30, 0, 0, // sample_rate = 12345 
            0xc9, 0x33, 0x03, 0, // byte_rate = 1 * 17 * 12345
            17, 0, // block_align = num_channels*bytes_per_sample
            1, 0, // bad bits_per_sample 
            'd', 'a', 't', 'a', // subchunk2Id
            0x99, 0xd0, 0x5b, 0x07, 
        };

        ReadableWaveBuffer r(badBitsPerSample, sizeof(badBitsPerSample));
        EXPECT_FALSE(readWaveHeader(&r, &num_channels, &sample_rate,
                       &bytes_per_sample, &num_samples));
    }

    {
        static const uint8_t badByteRate[] = {
            'R', 'I', 'F', 'F', // 
            0xbd, 0xd0, 0x5b, 0x07, // size_of_whole_file - 8 = 123457689 + 44 -8
            'W', 'A', 'V', 'E',
            'f', 'm', 't', ' ', // subchun1Id
            16, 0, 0, 0, // size_of_fmt - 8 = 24 - 8
            6, 0, // format = 6
            17, 0, // channels = 17
            0x39, 0x30, 0, 0, // sample_rate = 12345 
            0x00, 0x33, 0x03, 0, // bad byte_rate
            17, 0, // block_align = num_channels*bytes_per_sample
            8, 0, // bits_per_sample 
            'd', 'a', 't', 'a', // subchunk2Id
            0x99, 0xd0, 0x5b, 0x07, 
        };

        ReadableWaveBuffer r(badByteRate, sizeof(badByteRate));
        EXPECT_FALSE(readWaveHeader(&r, &num_channels, &sample_rate,
                       &bytes_per_sample, &num_samples));
    }

    {
        static const uint8_t badHeaderSize[] = {
            'R', 'I', 'F', 'F',
            0xbd, 0xd0, 0x5b, 0x07, // size_of_whole_file - 8 = 123457689 + 44 -8
            'W', 'A', 'V', 'E',
            'f', 'm', 't', ' ', // subchun1Id
            17, 0, 0, 0, // bad size_of_fmt - 8: 
            6, 0, // format = 6
            17, 0, // channels = 17
            0x39, 0x30, 0, 0, // sample_rate = 12345 
            0xc9, 0x33, 0x03, 0, // byte_rate = 1 * 17 * 12345
            17, 0, // block_align = num_channels*bytes_per_sample
            8, 0, // bits_per_sample
            'd', 'a', 't', 'a', // subchunk2Id
            0x99, 0xd0, 0x5b, 0x07, 
        };

        ReadableWaveBuffer r(badHeaderSize, sizeof(badHeaderSize));
        EXPECT_FALSE(readWaveHeader(&r, &num_channels, &sample_rate,
                       &bytes_per_sample, &num_samples));
    }

    {
        static const uint8_t typicalHeader[] = {
            'R', 'I', 'F', 'F',
            0x3d, 0xd1, 0x5b, 0x07, // size_of_whole_file - 8 = 123457689 + 44 -8
            'W', 'A', 'V', 'E',
            'f', 'm', 't', ' ', // subchun1Id
            16, 0, 0, 0, // size_of_fmt - 8 = 24 - 8
            6, 0, // format = 6
            17, 0, // channels = 17
            0x39, 0x30, 0, 0, // sample_rate = 12345 
            0xc9, 0x33, 0x03, 0, // byte_rate = 1 * 17 * 12345
            17, 0, // block_align = num_channels*bytes_per_sample
            8, 0, // bits_per_sample
            'd', 'a', 't', 'a', // subchunk2Id
            0x99, 0xd0, 0x5b, 0x07,
        };

        size_t num_channels = 0;
        int sample_rate = 0;
        size_t bytes_per_sample = 0;
        size_t num_samples = 0;

        ReadableWaveBuffer r(typicalHeader, sizeof(typicalHeader), false);
        EXPECT_TRUE(readWaveHeader(&r, &num_channels, &sample_rate,
                       &bytes_per_sample, &num_samples));
        
        EXPECT_EQ(17u, num_channels);
        EXPECT_EQ(12345, sample_rate);
        EXPECT_EQ(1u, bytes_per_sample);
        EXPECT_EQ(123457689u, num_samples);
    }

}

} // namespace wave
