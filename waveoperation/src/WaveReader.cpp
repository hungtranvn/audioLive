#include "WaveReader.hpp"
#include "ReadableWave.hpp"
#include "WaveHeader.hpp"
#include "WaveUtils.hpp"

namespace wave {
// TODO: update wavereader to check filename is valid before open and read
WaveReader::WaveReader(const std::string& filename) 
    : m_file_handle(fopen(filename.c_str(), "rb")) 
{
    ReadableWave readable(m_file_handle);
    size_t bytes_per_sample;
    if (readWaveHeader(&readable, &m_num_channels, &m_sample_rate, 
                   &bytes_per_sample, &m_num_samples))  
    {
    }
    m_num_samples_remaining = m_num_samples;

}

WaveReader::~WaveReader() {
    close();
}

auto WaveReader::getSampleRate() const -> int
{
    return m_sample_rate;
}

auto WaveReader::getNumChannels() const -> size_t
{
    return m_num_channels;
}

auto WaveReader::getNumSamples() const -> size_t
{
    return m_num_samples;
}

// TODO: using vector instead of int16_t*
auto WaveReader::readSamples(size_t num_samples, int16_t* samples) -> size_t
{
    num_samples = std::min(num_samples, m_num_samples_remaining);
    const size_t read = fread(samples, sizeof(*samples), num_samples, m_file_handle);
    m_num_samples_remaining -= read;
    return read;
}

auto WaveReader::readSamples(size_t num_samples, float* samples) -> size_t
{
    static const size_t chunkSize = 4096 / sizeof(uint16_t);
    size_t read = 0;
    
    for (size_t i = 0; i < num_samples; i += chunkSize)
    {
        int16_t isamples[chunkSize];
        size_t chunk = std::min(chunkSize, num_samples - i);
        chunk = readSamples(chunk, isamples);

        for (size_t j = 0; j < chunk; ++j)
            samples[i + j] = isamples[j];
        read += chunk;
    }
    return read;
}

auto WaveReader::close() -> void
{
    if (m_file_handle != nullptr)
    {
        fclose(m_file_handle);
        m_file_handle = nullptr;
    }
}
} // namespace wave
