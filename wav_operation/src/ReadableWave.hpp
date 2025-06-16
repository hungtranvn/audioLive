#ifndef READABLE_WAVE_FILE_HPP
#define READABLE_WAVE_FILE_HPP

#include "./interface/IReadableWave.hpp"

namespace wave {

static const size_t PCM_BYTES_PER_SAMPLE = 2;

class ReadableWaveFile : public IReadableWave {
public:
    explicit ReadableWaveFile(FILE* file) : m_file(file) {}
    virtual size_t read(void* buf, size_t num_bytes) {
        return fread(buf, 1, num_bytes, m_file);
    }
private:
    FILE* m_file;
};
} // namespace wave

#endif // READABLEWAVE_HPP
