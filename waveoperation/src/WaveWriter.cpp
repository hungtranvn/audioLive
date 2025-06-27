#include "WaveWriter.hpp"
#include "WaveHeader.hpp"
#include "WaveUtils.hpp"

#include <cstdint>
namespace wave {

WaveWriter::WaveWriter(const std::string& filename, int sample_rate, size_t num_channels) : 
    m_sample_rate(sample_rate),
    m_num_channels(num_channels),
    m_num_samples(0),
    m_file_handle(fopen(filename.c_str(), "wb"))
{
    if (m_file_handle == nullptr)
        std::cout << "Could not open wave file! \n";
    checkWaveParameters(m_num_channels, m_sample_rate, BYTES_PER_SAMPLE, m_num_samples);
    static const uint8_t header[WAVE_HEADER_SIZE] = {0};
    fwrite(header, WAVE_HEADER_SIZE, 1, m_file_handle);
}

WaveWriter::~WaveWriter() 
{
    close();
}

auto WaveWriter::getSampleRate() const -> int
{
    return m_sample_rate;
}

auto WaveWriter::getNumChannels() const -> size_t
{
    return m_num_channels;
}

auto WaveWriter::getNumSamples() const -> size_t 
{
    return m_num_samples;
}

auto WaveWriter::writeSamples(const int16_t* samples, size_t num_samples) -> void
{
    
}

auto WaveWriter::writeSamples(const float* samples, size_t num_samples) -> void
{

}

auto WaveWriter::close() -> void
{
    fseek(m_file_handle, 0, SEEK_SET);
    uint8_t header[WAVE_HEADER_SIZE];
    writeWaveHeader(header, m_num_channels, m_sample_rate,
                    BYTES_PER_SAMPLE, m_num_samples);
    fwrite(header, WAVE_HEADER_SIZE, 1, m_file_handle);
    fclose(m_file_handle);
    m_file_handle = nullptr;
}

} // namespace wave
