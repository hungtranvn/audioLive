#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/WaveWriter.hpp"

#include <string>
#include <iostream>
#include <vector>

namespace wave {

TEST(WaveWriteReaderTest, simpleWriteReaderTest) {
    const std::string output = "wavetest.wav";
    static const size_t num_samples = 1;
    
    {
        WaveWriter w(output, 14099, 2);
        EXPECT_EQ(14099, w.getSampleRate());
        EXPECT_EQ(1u, w.getNumChannels());
        EXPECT_EQ(0u, w.getNumSamples());
//w.writeSamples(num_samples, w.getNumSamples());
    }
    
}

} // namespace wave
