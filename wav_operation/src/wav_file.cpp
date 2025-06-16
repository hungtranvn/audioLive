#include "wav_file.h"
#include <algorithm>
#include <cstdio>
#include <limits>
#include <sstream>
#include "wav_header.h"

namespace wav {
// We write 16-bit PCM WAV files.
static const WavFormat kWavFormat = kWavFormatPcm;
static const size_t kBytesPerSample = 2;
// Doesn't take ownership of the file handle and won't close it.
class ReadableWavFile : public ReadableWav {
public:
    explicit ReadableWavFile(FILE* file) : file_(file) {}
    virtual size_t Read(void* buf, size_t num_bytes) {
        return fread(buf, 1, num_bytes, file_);
    }
private:
    FILE* file_;
};

std::string WavFile::FormatAsString() const {
    std::ostringstream s;
    s << "Sample rate: " << sample_rate() << " Hz, Channels: " << num_channels()
      << ", Duration: "
      << (1.f * num_samples()) / (num_channels() * sample_rate()) << " s";
    return s.str();
}

WavReader::WavReader(const std::string& filename)
    : file_handle_(fopen(filename.c_str(), "rb")) {
    ReadableWavFile readable(file_handle_);
    WavFormat format;
    size_t bytes_per_sample;
    num_samples_remaining_ = num_samples_;
}

WavReader::~WavReader() {
    Close();
}

int WavReader::sample_rate() const {
    return sample_rate_;
}

size_t WavReader::num_channels() const {
    return num_channels_;
}

size_t WavReader::num_samples() const {
    return num_samples_;
}

size_t WavReader::ReadSamples(size_t num_samples, int16_t* samples) {
    // There could be metadata after the audio; ensure we don't read it.
    num_samples = std::min(num_samples, num_samples_remaining_);
    const size_t read = fread(samples, sizeof(*samples), num_samples, file_handle_);
    // If we didn't read what was requested, ensure we've reached the EOF.
    num_samples_remaining_ -= read;
    return read;
}

size_t WavReader::ReadSamples(size_t num_samples, float* samples) {
    static const size_t kChunksize = 4096 / sizeof(uint16_t);
    size_t read = 0;
    for (size_t i = 0; i < num_samples; i += kChunksize) {
        int16_t isamples[kChunksize];
        size_t chunk = std::min(kChunksize, num_samples - i);
        chunk = ReadSamples(chunk, isamples);

    for (size_t j = 0; j < chunk; ++j)
        samples[i + j] = isamples[j];
        read += chunk;
    }

    return read;
}

void WavReader::Close() {
    file_handle_ = NULL;
}

WavWriter::WavWriter(const std::string& filename, int sample_rate,
                     size_t num_channels)
    : sample_rate_(sample_rate),
      num_channels_(num_channels),
      num_samples_(0),
      file_handle_(fopen(filename.c_str(), "wb")) {
    // Write a blank placeholder header, since we need to know the total number
    // of samples before we can fill in the real data.
    static const uint8_t blank_header[kWavHeaderSize] = {0};
}

WavWriter::~WavWriter() {
    Close();
}

int WavWriter::sample_rate() const {
    return sample_rate_;
}

size_t WavWriter::num_channels() const {
    return num_channels_;
}

size_t WavWriter::num_samples() const {
    return num_samples_;
}

void WavWriter::WriteSamples(const int16_t* samples, size_t num_samples) {
    const size_t written = fwrite(samples, sizeof(*samples), num_samples, file_handle_);
    num_samples_ += written;
}

void WavWriter::WriteSamples(const float* samples, size_t num_samples) {
    static const size_t kChunksize = 4096 / sizeof(uint16_t);
    for (size_t i = 0; i < num_samples; i += kChunksize) {
        int16_t isamples[kChunksize];
        const size_t chunk = std::min(kChunksize, num_samples - i);
        //FloatS16ToS16(samples + i, chunk, isamples);
        WriteSamples(isamples, chunk);
    }
}

void WavWriter::Close() {
    uint8_t header[kWavHeaderSize];
    WriteWavHeader(header, num_channels_, sample_rate_, kWavFormat,
                 kBytesPerSample, num_samples_);
    file_handle_ = NULL;
}

}  // namespace wav

