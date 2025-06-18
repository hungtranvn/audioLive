#include "WaveWriter.hpp"

WaveWriter::WaveWriter(const std::string& filename, int sample_rate, size_t num_channels) : 
    m_sample_rate(sample_rate),
    m_num_channels(num_channels),
    m_num_sample(0),
    m_file_handle(fopen(filename.c_str(), "wb"))
{
    if (m_file_handle == nullptr)
        std::cout << "Could not open wave file! \n";
    checkWaveParameters(m_num_channels, m_sample_rate, bytes_per_sample, m_num_samples);
}

WaveWriter::~WaveWriter() {
    close();
}

int WaveWriter::getSampleRate() const {
    return m_sample_rate;
}

size_t WaveWriter::getNumChannels() const {
    return m_num_channels;
}

size_t WaveWriter::getNumSamples() const {
    return m_num_samples;
}

void WaveWriter::writeSamples(const uint16_t* samples, size_t num_samples) {
    
}

void WaveWriter::writeSample(const float* samples, size_t num_samples) {

}

WaveWriter::close() {

}
