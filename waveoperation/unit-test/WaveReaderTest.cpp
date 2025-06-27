#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/WaveReader.hpp"
#include "../src/WaveWriter.hpp"

#include <string>
#include <iostream>
#include <vector>

namespace wave {

std::string test_file = "/home/run4hc/Workspace/audioLive/waveoperation/unit-test/audio_file/Testsequence_Speech-Music_48kHz_10min.wav";

TEST(WaveWriteReaderTest, simpleWriteReaderTest) {
    const std::string output = "wavetest.wav";
    static const size_t num_samples = 1;
    
    {
        WaveWriter w(output, 14099, 2);
        EXPECT_EQ(14099, w.getSampleRate());
        EXPECT_EQ(1u, w.getNumChannels());
        EXPECT_EQ(0u, w.getNumSamples());
        w.writeSamples(num_samples, w.getNumSamples());
    }

    WaveReader reader(test_file);
    {
        std::cout << "Get samples rate: " << reader.getSampleRate() << std::endl;    
        std::cout << "Get num channels: " << reader.getNumChannels() << std::endl;    
        std::cout << "Get num samples: " << reader.getNumSamples() << std::endl;    
        EXPECT_EQ(1 + 1, 2);
    }
    
    {
        int16_t samples[102];
        size_t num_sample = 100;
        size_t ret = reader.readSamples(num_sample, samples);
        for (auto i = 0; i <= 100; ++i) {
            std::cout << samples[i] << " ";
        }
        
    }
}

TEST(WaveReaderTest, simpleReaderTest) {
    WaveReader reader(test_file);
    {
        std::cout << "Get samples rate: " << reader.getSampleRate() << std::endl;    
        std::cout << "Get num channels: " << reader.getNumChannels() << std::endl;    
        std::cout << "Get num samples: " << reader.getNumSamples() << std::endl;    
        EXPECT_EQ(1 + 1, 2);
    }
    
    {
        int16_t samples[102];
        size_t num_sample = 100;
        size_t ret = reader.readSamples(num_sample, samples);
        for (auto i = 0; i <= 100; ++i) {
            std::cout << samples[i] << " ";
        }
        
    }
}


} // namespace wave
