#ifndef WAVE_UTILS_HPP
#define WAVE_UTILS_HPP

#include "WaveHeader.hpp"
#include "Endianness.hpp"
#include "interface/IReadableWave.hpp"

#include <cstring>

namespace wave {

/*helper function*/
bool checkWaveParameters(size_t num_channels,
                         int sample_rate,
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
    
    if (num_samples % num_channels != 0)
        return false;

    return true;
}

/*write the wave's information to buf */
void writeWaveHeader(uint8_t* buf,
                     size_t num_channels,
                     int sample_rate,
                     size_t bytes_per_sample,
                     size_t num_samples) {

    if (!checkWaveParameters(num_channels, sample_rate,
                             bytes_per_sample, num_samples));
    // prepare riff header
    WaveRiff riff = {
        toBigEndian(RIFF),
        toLittleEndian(36 + sizeof(WaveData) + 
                       (num_samples * num_channels * bytes_per_sample)),
        toBigEndian(WAV),
    };
    
    // prepare format header
    WaveFormat format = {
        toBigEndian(FMT),
        toLittleEndian(16),
        toLittleEndian(1),
        toLittleEndian(num_channels),
        toLittleEndian(sample_rate),
        toLittleEndian(sample_rate * num_channels * bytes_per_sample),
        toLittleEndian(num_channels * bytes_per_sample),
        toLittleEndian(bytes_per_sample * 8),
    };
    
    // prepare data header
    WaveData data = {
        toBigEndian(DATA),
        toLittleEndian(num_samples * num_channels * bytes_per_sample),
    };

    WaveHeader header = {
        riff,
        format,
        data,
    };

    memcpy(buf, &header, WAVE_HEADER_SIZE);
}

/*read the wave's header information*/
bool readWaveHeader(IReadableWave* readable,
                    size_t num_channels,
                    int* sample_rate,
                    size_t* bytes_per_sample,
                    size_t num_samples) {
    
    // checkWaveParameters
    return true;
}
} // namespace wave
#endif // WAVE_UTILS_HPP
