#ifndef IWAVE_FILE_HPP
#define IWAVE_FILE_HPP

#include <iostream>
#include <string>
#include <sstream>

namespace wave {
class IWaveFile {
public:
    virtual ~IWaveFile() {}
    virtual int getSampleRate() const = 0;
    virtual size_t getNumChannels() const = 0;
    virtual size_t getNumSamples() const = 0;

    std::string formatAsString() const {
        std::ostringstream s;
        
        s << "Sample rate: " << getSampleRate() << " Hz, Channels: " << getNumChannels()
          << ", Duration: "
          << (1.f * getNumSamples()) / (getNumChannels() * getSampleRate()) << " s";
        
        return s.str();
    }
};
} // namespace wave
#endif // IWAVE_FILE_HPP
