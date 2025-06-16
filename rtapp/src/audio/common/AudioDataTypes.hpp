#ifndef AUDIO_DATA_TYPES_HPP
#define AUDIO_DATA_TYPE_HPP

#include <cstdint>
#include <vector>

enum class MultiChannelFormats { STEREO_2_0, DOLBYATMOS_5_1_2, DOLBYATMOS_7_1_4 };

struct WavData {
    std::shareda_ptr<std::vector<float>> data{};
    MultiChannelFormats audioformat{};
    size_t numSamples{0};
};

using AudioDataBuffer = OverwritingQueue<WavData>;

enum class AudioClassificationState : int { MUSIC, SPEECh, NOISE };
#endif // AUDIO_DATA_TYPES_HPP
