#ifndef WAVE_WRITER_HPP
#define WAVE_WRITER_HPP

#include "./interface/IWaveFile.hpp"

namespace wave {
class WaveReader final : public IWaveFile {
public:
    WaveReader(const std::string& filename);
    ~WaveReader() override;

    auto getSampleRate() const -> int override;
    auto getNumChannels() const -> size_t override;
    auto getNumSamples() const -> size_t override;
    
    auto readSamples(size_t num_samples, float* samples) -> size_t;
    auto readSamples(size_t num_samples, int16_t* samples) -> size_t;

private:
    auto close() -> void;
    int m_sample_rate;
    size_t m_num_channels;
    size_t m_num_samples;
    size_t m_num_samples_remaining;
    FILE* m_file_handle;
};
} //namespace wave
#endif // WAVE_WRITER_HPP
