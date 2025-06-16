#ifndef WAVE_UTILS_HPP
#define WAVE_UTILS_HPP

#include "Wave.hpp"
#include "Endianness.hpp"

namespace wave {

/*helper function*/
bool checkWaveParameters(size_t num_channels,
                         int sample_rate,
                         WaveFormat format,
                         size_t bytes_per_sample,
                         size_t num_samples) {
    if (num_channels == 0 || sample_rate <= 0 || bytes_per_sample == 0)
        return false;
    if (static_cast<uint64_t>(sample_rate) > std::numeric_limits<uint32_t>::max())
        return false;
    if (num_channels > std::numeric_limits<uint16_t>::max())
        return false;
    if (static_cast<uint64_t>(bytes_per_sample) * 8 > std::numeric_limits<uint16_t>::max())
        return false;
    if (static_cast<uint64_t>(sample_rate) * num_channels * bytes_per_sample > 
                                          std::numeric_limits<uint32_t>::max())
        return false;

    /*format and bytes_per_sample must agree*/
    switch (format) {
        case WAVE_FORMAT_PCM:
            if (bytes_per_sample != 1 && bytes_per_sample != 2)
                return false;
            break;
        case WAVE_FORMAT_ALAW:
        case WAVE_FORMAT_MULAW:
            if (bytes_per_sample != 1)
                return false;
            break;
        default:
            return false;
    }
    
    if (num_samples % num_channels != 0)
        return false;
}

/*write the wave's information to buf */
void writeWaveHeader(uint8_t* buf,
                     size_t num_channels,
                     int sample_rate,
                     WaveFormat format,
                     size_t bytes_per_sample,
                     size_t num_samples) {
    if (!checkWaveParameters(num_channels, sample_rate, format,
                             bytes_per_sample, num_samples));
    WaveHeader header;
    const size_t bytes_in_payload = bytes_per_sample * num_samples;
    // write riff
    WaveRiff riff = {
        toBigEndian(RIFF),
        toLittle(),
        toBigEndian(WAV)
    }
    
}

/*read the wave's header information*/
bool readWaveHeader(ReadableWav* readable,
                    size_t num_channels,
                    int* sample_rate,
                    WaveFormat* format,
                    size_t* bytes_per_sample,
                    size_t num_samples) {

    // checkWaveParameters
}
} // namespace wave
#endif // WAVE_UTILS_HPP
