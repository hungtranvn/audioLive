#ifndef I_READABLE_WAVE_HPP
#define I_READBLE_WAVE_HPP
namespace wave {

class IReadableWave {
public:
    size_t virtual read(void* buf, size_t num_bytes) = 0;
    virtual ~IReadableWave() {}
};

}// namespace wave
#endif //I_READABLE_WAVE_HPP
