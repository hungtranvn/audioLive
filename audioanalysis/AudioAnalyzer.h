#ifndef AUDIO_ANALYZER_HPP
#define AUDIO_ANALYZER_HPP

#include <complex>
#include <vector>
#include <algorithm>

namespace audio {
constexpr auto getPi() -> double
{
    constexpr double constPi = 3.14159265358979323846;
    return constPi;
}

inline auto hammingWindow(unsigned int n, unsigned int windowSize) -> float
{
    constexpr float coeffA0 = 0.54F;
    constexpr float coeffA1 = 0.46F;
    constexpr double piPi = 2.0 * getPi();

    return coeffA0 - coeffA1 * static_cast<float>(cos(piPi * static_cast<double>(n) / static_cast<double>(windowSize - 1)));
}

class AudioAnalyzer {
public:
    AudioAnalyzer(void);
    ~AudioAnalyzer() = default;

    auto setSampleRate(uint32_t sampleRate) -> uint32_t;
    auto addSamples(const std::vector<float>& vStereoSamples) -> uint32_t;
    void addPowerSpectrum(const std::vector<float>& bins);
private:
    unsigned int m_sampleRate{0}
    unsigned int m_fftSize;
};

}

#endif // AUDIO_ANALYZER_HPP
