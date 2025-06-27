#ifndef WAVE_UTILS_HPP
#define WAVE_UTILS_HPP

#include "WaveHeader.hpp"
#include "Endianness.hpp"
#include "./ReadableWave.hpp"

#include <cstring>
#include <limits>
#include <iostream>

namespace wave {


/*Wave Header suport function*/
/*helper function*/
auto checkWaveParameters(size_t num_channels,
                         int sample_rate,
                         size_t bytes_per_sample,
                         size_t num_samples) -> bool
{
    if (num_channels == 0 || sample_rate <= 0 || bytes_per_sample == 0) {
        std::cout << "check 0" << std::endl;
        return false;
    }
    if (static_cast<uint64_t>(sample_rate) > std::numeric_limits<uint32_t>::max()) {
        std::cout << "check limit" << std::endl;
        return false;
    }
    if (num_channels > std::numeric_limits<uint16_t>::max()) {
        std::cout << "check limit" << std::endl;
        return false;
    }
    if (static_cast<uint64_t>(bytes_per_sample) * 8 > std::numeric_limits<uint16_t>::max()) {
        std::cout << "check limit" << std::endl;
        return false;
    }
    if (static_cast<uint64_t>(sample_rate) * num_channels * bytes_per_sample > 
                                          std::numeric_limits<uint32_t>::max()) {
        std::cout << "check limit" << std::endl;
        return false;
    }
    
    if (num_samples % num_channels != 0) {
        std::cout << "check valid" << std::endl;
        return false;
    }
    std::cout << "pass!!!!!!!" << std::endl;
    return true;
}

/*write the wave's information to buf */
auto writeWaveHeader(uint8_t* buf,
                     size_t num_channels,
                     int sample_rate,
                     size_t bytes_per_sample,
                     size_t num_samples) -> void
{

    if (!checkWaveParameters(num_channels, sample_rate,
                             bytes_per_sample, num_samples));
    // prepare riff header
    WaveRiff riff = {
        toBigEndian(RIFF),
        toLittleEndian(static_cast<uint32_t> (36 + sizeof(WaveData) + 
                       (num_samples * num_channels * bytes_per_sample))),
        toBigEndian(WAV),
    };
    
    // prepare format header
    WaveFormat format = {
        toBigEndian(FMT),
        toLittleEndian(static_cast<uint32_t>(16)),
        toLittleEndian(static_cast<uint16_t>(1)),
        toLittleEndian(num_channels),
        toLittleEndian(sample_rate),
        toLittleEndian(sample_rate * num_channels * bytes_per_sample),
        toLittleEndian(static_cast<uint16_t>(num_channels * bytes_per_sample)),
        toLittleEndian(static_cast<uint16_t>(bytes_per_sample * 8)),
    };
    
    // prepare data header
    WaveData data = {
        toBigEndian(DATA),
        toLittleEndian(static_cast<uint32_t>(num_samples * num_channels * bytes_per_sample)),
    };

    WaveHeader header = {
        riff,
        format,
        data,
    };

    memcpy(buf, &header, WAVE_HEADER_SIZE);
}

/*read the wave's header information*/
auto readWaveHeader(ReadableWave* readable,
                    size_t* num_channels,
                    int* sample_rate,
                    size_t* bytes_per_sample,
                    size_t* num_samples) -> bool
{
    WaveHeader header;
    readable->read(&header, WAVE_HEADER_SIZE);
    
    // Parse the fields
    *num_channels = toLittleEndian(header.waveFmt.numChannels);
    *sample_rate = toLittleEndian(header.waveFmt.sampleRate);
    *bytes_per_sample = toLittleEndian(header.waveFmt.bitsPerSample) / 8;
    std::cout << "bytes_per_sample: " << *bytes_per_sample << std::endl;
    if (*bytes_per_sample == 0)
        return false;
    const size_t bytes_in_payload = toLittleEndian(header.waveData.subchunk2Size);
    std::cout << "bytes_in_payload: " << bytes_in_payload 
        << " bytes_per_sample: "  << *bytes_per_sample;
    *num_samples = bytes_in_payload / *bytes_per_sample;
    std::cout << " num_samples: " << *num_samples << std::endl;
 
    // sanity check
    uint32_t chunkId = header.waveRiff.chunkId;
    if (chunkId != RIFF) {
        std::cout<< "chunkId: " << std::hex << chunkId << std::endl;    
        return false;
    }
    
    uint32_t format = header.waveRiff.format;
    if (format != WAV) {
        std::cout<< "format: " << std::hex << format << std::endl;    
        return false;
    }
    
    uint32_t subchunk1Id = header.waveFmt.subchunk1Id;
    if (subchunk1Id != FMT) {
        std::cout<< "subchun1Id: " << std::hex << subchunk1Id << std::endl;    
        return false;
    }
    
    uint32_t subchunk2Id = header.waveData.subchunk2Id;
    if (subchunk2Id != DATA) {
        std::cout<< "subchunk2Id: " << std::hex << subchunk2Id << std::endl;    
        return false;
    }
  
    return checkWaveParameters(*num_channels, *sample_rate,
                               *bytes_per_sample, *num_samples);
}
// end wave header suport function

} // namespace wave
#endif // WAVE_UTILS_HPP
