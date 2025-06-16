#ifndef IAUDIODATAEXCHANGE_HPP
#define IAUDIODATAEXCHANGE_HPP

#include <memory>

class IAudioDataExchange {
public:
    virtual ~IAudioDataExchange() = default;
    virtual auto setAudioDataBufferr(std::shared_ptr<AudioDataBuffer> audioDataBuffer) -> void = 0;
};
#endif // IAUDIODATAEXCHANGE_HPP
