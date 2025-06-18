#ifndef WAVE_HPP
#define WAVE_HPP

#include <stdio.h>
#include <stdint.h>

namespace wave {
// ref: http://soundfile.sapp.org/doc/WaveFormat/

enum WaveFormat {
    WAVE_FORMAT_PCM     = 1,
    WAVE_FORMAT_ALAW    = 6,
    WAVE_FORMAT_MULAW   = 7,
};

/*RIFF header*/
struct WaveRiff {
    uint32_t chunkId;
    uint32_t chunkSize;
    uint32_t format;
};

/*Format header*/
struct WaveFormat {
    uint32_t subchunk1Id;
    uint32_t subchunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bytesPerSample;
};

/*Data header*/
struct WaveData {
    uint32_t subchunk2Id;
    uint32_t subchunk2Size;
};

/*Completed header*/
struct WaveHeader {
    WaveRiff waveRiff;
    WaveFormat waveFmt;
    WaveData waveData;
};

/*Sample struct*/
struct WaveSample {
    unsigned int channels;
    unsigned int bytesPerSample;
    unsigned int dataSize;
    void *sampleData;
};

static const int WAVE_HEADER_SIZE = 44;
static_assert(sizeof(WaveHeader) == WAVE_HEADER_SIZE, "No padding in header");

static uint32_t RIFF = 0x52494646;
static uint32_t WAV = 0x57415645;
static uint32_t FMT = 0x666d7420;
static uint32_t DATA = 0x64617461;

};
#endif // WAVE_HPP
