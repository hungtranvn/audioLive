#ifndef WAV_HEADER_H
#define WAV_HEADER_H
#include <stddef.h>
#include <stdint.h>
// ref:  https://ccrma.stanford.edu/courses/422-winter-2014/projects/WaveFormat/
namespace wav {
static const size_t kWavHeaderSize = 44;

class ReadableWav {
public:
    // Returns the number of bytes read.
    size_t virtual Read(void* buf, size_t num_bytes) = 0;
    virtual ~ReadableWav() {}
};

enum WavFormat {
    kWavFormatPcm   = 1,  // PCM, each sample of size bytes_per_sample
    kWavFormatALaw  = 6,  // 8-bit ITU-T G.711 A-law
    kWavFormatMuLaw = 7,  // 8-bit ITU-T G.711 mu-law
    kWavFormatUnknown,
};

// Return true if the given parameters will make a well-formed WAV header.
bool CheckWavParameters(size_t num_channels,
                        int sample_rate,
                        WavFormat format,
                        size_t bytes_per_sample,
                        size_t num_samples);

// Write a kWavHeaderSize bytes long WAV header to buf. The payload that
// follows the header is supposed to have the specified number of interleaved
// channels and contain the specified total number of samples of the specified
// type. CHECKs the input parameters for validity.
void WriteWavHeader(uint8_t* buf,
                    size_t num_channels,
                    int sample_rate,
                    WavFormat format,
                    size_t bytes_per_sample,
                    size_t num_samples);

// Read a WAV header from an implemented ReadableWav and parse the values into
// the provided output parameters. ReadableWav is used because the header can
// be variably sized. Returns false if the header is invalid.
bool ReadWavHeader(ReadableWav* readable,
                   size_t* num_channels,
                   int* sample_rate,
                   WavFormat* format,
                   size_t* bytes_per_sample,
                   size_t* num_samples);
}  // namespace wav
#endif //WAV_HEADER_H
