#ifndef WAVE_WRITER_HPP
#define WAVE_WRITER_HPP

#include "./interface/IWaveFile.hpp"
class WaveWriter final : public IWaveFile {
public:
    WaveWriter(const std::string& filename, int sample_rate, size_t num_channels);
    ~WaveWriter() override;

    void WriteSamples(const float* samples, size_t num_samples);
    void WriteSample(const int16_t* samples, size_t num_samples);

    int getSampleRate() const override;
    size_t getNumChannels() const override;
    size_t getNumSamples() const override;
private:
    void close();
    int m_sample_rate;
    size_t m_num_channels;
    size_t m_num_samples;
    size_t m_num_samples_remaining;
    FILE* m_file_handle;
}

#endif // WAVE_WRITER_HPP
