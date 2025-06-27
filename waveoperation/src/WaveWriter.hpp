#ifndef WAVE_WRITER_HPP
#define WAVE_WRITER_HPP

#include "./interface/IWaveFile.hpp"
namespace wave {
class WaveWriter final : public IWaveFile {
public:
    WaveWriter(const std::string& filename, int sample_rate, size_t num_channels);
    ~WaveWriter() override;

    void writeSamples(const float* samples, size_t num_samples);
    void writeSamples(const int16_t* samples, size_t num_samples);

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
};
} // namespace wave
#endif // WAVE_WRITER_HPP
