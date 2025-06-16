#include "../src/wav_header.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <limits>

namespace wav {

class ReadableWavBuffer : public ReadableWav {
public:
    ReadableWavBuffer(const uint8_t* buf, size_t size)
        : buf_(buf), size_(size), pos_(0),
          buf_exhausted_(false), check_read_size_(true) { }
    ReadableWavBuffer(const uint8_t* buf, size_t size, bool check_read_size)
        : buf_(buf),
          size_(size),
          pos_(0),
          buf_exhausted_(false),
          check_read_size_(check_read_size) { }
    virtual ~ReadableWavBuffer() {
        if (check_read_size_)
            EXPECT_EQ(size_, pos_);
    }

    virtual size_t Read(void* buf, size_t num_bytes) {
        if (size_ >= kWavHeaderSize)
            EXPECT_GE(size_, pos_ + num_bytes);
        EXPECT_FALSE(buf_exhausted_);

        const size_t bytes_remaining = size_ - pos_;
        if (num_bytes > bytes_remaining) {
            buf_exhausted_ = true;
            num_bytes = bytes_remaining;
        }
        memcpy(buf, &buf_[pos_], num_bytes);
        pos_ += num_bytes;
        return num_bytes;
    }
private:
    const uint8_t* buf_;
    const size_t size_;
    size_t pos_;
    bool buf_exhausted_;
    const bool check_read_size_;
};

// CheckWavParameters function using parameterized test case
struct WavParamCase {
    size_t num_channels;
    int sample_rate;
    WavFormat format;
    size_t bytes_per_sample;
    size_t num_samples;
    bool expected;
};

class CheckWavParametersTest : public ::testing::TestWithParam<WavParamCase> {};

TEST_P(CheckWavParametersTest, HandlesVariousInputs) {
    const auto& p = GetParam();
    EXPECT_EQ(CheckWavParameters(p.num_channels,
                               p.sample_rate,
                               p.format,
                               p.bytes_per_sample,
                               p.num_samples),
                               p.expected);
}

INSTANTIATE_TEST_SUITE_P (
    AllCases,
    CheckWavParametersTest,
    ::testing::Values(
        WavParamCase{0, 44100, kWavFormatPcm, 2, 1000, false},
        WavParamCase{2, 0, kWavFormatPcm, 2, 1000, false}, // sample_rate == 0
        WavParamCase{2, 44100, kWavFormatPcm, 0, 1000, false}, // bytes_per_sample == 0
        WavParamCase{2, static_cast<int>(std::numeric_limits<uint32_t>::max()) + 1, kWavFormatPcm, 2, 1000, false}, // sample_rate > uint32_t max
        WavParamCase{std::numeric_limits<uint16_t>::max()+1, 44100, kWavFormatPcm, 2, 1000, false}, // num_channels > uint16_t max
        WavParamCase{2, 44100, kWavFormatPcm, (std::numeric_limits<uint16_t>::max()/8) + 1, 1000, false}, // bits_per_sample > uint16_t max
        WavParamCase{2, 0x10000, kWavFormatPcm, 16384, 1000, false}, // byte_rate > uint32_t max
        WavParamCase{2, 44100, kWavFormatPcm, 3, 1000, false}, // PCM, invalid bytes_per_sample
        WavParamCase{2, 44100, kWavFormatPcm, 1, 1000, true},  // PCM, valid
        WavParamCase{2, 44100, kWavFormatPcm, 2, 1000, true},  // PCM, valid
        WavParamCase{2, 44100, kWavFormatALaw,  2, 1000, false}, // A-Law, invalid
        WavParamCase{2, 44100, kWavFormatALaw,  1, 1000, true},  // A-Law, valid
        WavParamCase{2, 44100, kWavFormatMuLaw, 2, 1000, false}, // Mu-Law, invalid
        WavParamCase{2, 44100, kWavFormatMuLaw, 1, 1000, true},  // Mu-Law, valid
        WavParamCase{2, 44100, kWavFormatUnknown, 2,  1000, false},  // Unknown format
        WavParamCase{3, 8000, kWavFormatPcm, 1, 5, false}
  )
);

// ReadableWavBuffer function
TEST(WavHeaderTest, ReadWavHeaderWithErrors) {
    size_t num_channels = 0;
    int sample_rate = 0;
    WavFormat format = kWavFormatPcm;
    size_t bytes_per_sample = 0;
    size_t num_samples = 0;

    {
        static const uint8_t kBadRiffID[] = {
            'R', 'i', 'f', 'f',  // *BAD*
            0xbd, 0xd0, 0x5b, 0x07,  // size of whole file - 8: 123457689 + 44 - 8
            'W', 'A', 'V', 'E',
            'f', 'm', 't', ' ',
            16, 0, 0, 0,  // size of fmt block - 8: 24 - 8
            6, 0,  // format: A-law (6)
            17, 0,  // channels: 17
            0x39, 0x30, 0, 0,  // sample rate: 12345
            0xc9, 0x33, 0x03, 0,  // byte rate: 1 * 17 * 12345
            17, 0,  // block align: NumChannels * BytesPerSample
            8, 0,  // bits per sample: 1 * 8
            'd', 'a', 't', 'a',
            0x99, 0xd0, 0x5b, 0x07,  // size of payload: 123457689
        };

        ReadableWavBuffer r(kBadRiffID, sizeof(kBadRiffID));
        EXPECT_FALSE(ReadWavHeader(&r, &num_channels, &sample_rate, &format,
                                   &bytes_per_sample, &num_samples));
    }
}

TEST(WavHeaderTest, WriteAndReadWavHeader) {
    static const int kSize = 4 + kWavHeaderSize + 4;
    uint8_t buf[kSize];
    memset(buf, 0xa4, sizeof(buf));
    WriteWavHeader(buf + 4, 17, 12345, kWavFormatALaw, 1, 123457689);
    static const uint8_t kExpectedBuf[] = {
        0xa4, 0xa4, 0xa4, 0xa4, // untouched bytes before header
        'R', 'I', 'F', 'F',
        0xbd, 0xd0, 0x5b, 0x07,
        'W', 'A', 'V', 'E',
        'f', 'm', 't', ' ',
        16, 0, 0, 0,
        6, 0,
        17, 0,
        0x39, 0x30, 0, 0,
        0xc9, 0x33, 0x33, 0,
        17, 0,
        8, 0,
        'd', 'a', 't', 'a',
        0x99, 0xd0, 0x5b, 0x07,
        0xa4, 0xa4, 0xa4, 0xa4, //untouched bytes after header
    };

    static_assert(sizeof(kExpectedBuf) == kSize, "buffer size");
    EXPECT_EQ(0, memcmp(kExpectedBuf, buf, kSize));

    size_t num_channels = 0;
    int sample_rate = 0;
    WavFormat format = kWavFormatPcm;
    size_t bytes_per_sample = 0;
    size_t num_samples = 0;
    ReadableWavBuffer r(buf + 4, sizeof(buf) - 8);
    EXPECT_TRUE(ReadWavHeader(&r, &num_channels, &sample_rate, &format,
                              &bytes_per_sample, &num_samples));
    EXPECT_EQ(17u, num_channels);
    EXPECT_EQ(12345, sample_rate);
    EXPECT_EQ(kWavFormatALaw, format);
    EXPECT_EQ(1u, bytes_per_sample);
    EXPECT_EQ(123457689u, num_samples);
}

TEST(WavHeaderTest, ReadATypicalWavHeader) {
    static const uint8_t kBuf[] = {
        'R', 'I', 'F', 'F',
        0x3d, 0xd4, 0x5b, 0x07,
        'W', 'A', 'V', 'E',
        'f', 'm', 't', ' ',
        18, 0 , 0, 0,
        6, 0,
        17, 0,
        0x39, 0x30, 0, 0,
        0xc9, 0x33, 0x03, 0,
        17, 0,
        8, 0,
        0, 0,
        'd', 'a', 't', 'a',
        0x99, 0xd0, 0x5b, 0x07,
    };

    size_t num_channels = 0;
    int sample_rate = 0;
    WavFormat format = kWavFormatPcm;
    size_t bytes_per_sample = 0;
    size_t num_samples = 0;
    ReadableWavBuffer r(kBuf, sizeof(kBuf));
    EXPECT_TRUE(ReadWavHeader(&r, &num_channels, &sample_rate, &format,
                              &bytes_per_sample, &num_samples));
    EXPECT_EQ(17u, num_channels);
    EXPECT_EQ(12345, sample_rate);
    EXPECT_EQ(kWavFormatALaw, format);
    EXPECT_EQ(1u, bytes_per_sample);
    EXPECT_EQ(123457689u, num_samples);
}
} // namspace wav
