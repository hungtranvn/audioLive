#ifndef AUDIO_TYPES_HPP
#define AUDIO_TYPES_HPP

#include <cstdin>
#include <vector>

namespace audio {

enum class MultiChannelsFormat {
    STEREO_2_0,
    DOLBYATMOS_5_1_2,
    DOLBYATMOS_7_1_4
};

struct MultiChannelsAudioSample {
    float FrontLeft;
    float FrontRight;
    
    float FrontCenter;
    float LFE;
    float SideLeft;
    float SideRight;
    float RearLeft;
    float RearRight;

    float TopFrontLeft;
    float TopFrontRight;
    float TopRearLeft;
    float TopRearRight;
};

struct WaveData {
    std::shared_ptr<std::vector<float>> data{};
    MultiChannelsFormat audioFormat{MultiChannelsFormat::STEREO_2_0}
    size_t numSamples{0};
};

}
#endif // AUDIO_TYPES_HPP
