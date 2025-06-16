#include "WaveReader.hpp"
#include "ReadableWave.hpp"

WaveReader::WaveReader(const std::string& filename) : m_file_handle(fopen(filename.c_str(), "rb")) {
    ReadableWave readable(m_file_handle);

}

WaveReader::~WaveReader() {
    close();
}

WaveReader::getSampleRate() const {
    return m_sample_rate;
}

WaveReader::getNumChannels() const {
    return m_num_channels;
}

WaveReader::getNumSamples() const {
    return m_num_samples;
}

size_t WaveReader::readSamples(size_t num_samples, int16_t samples) {

}

size_t WaveReader::readSamples(size_t num_samples, float* samples) {

}

void WaveReader::close() {
    fclose(m_file_handle);
    m_file_handle = nullptr;
}
