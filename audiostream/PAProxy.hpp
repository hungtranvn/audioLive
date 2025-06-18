#pragma once

#include <pulse/pulseaudio.h>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
#include <thread>

class PAProxy {
public:
    PAProxy();
    ~PAProxy();

    bool initialize(const std::string& sourceName);
    void shutdown();
    std::vector<uint8_t> fetchLatestSamples();

private:
    static void streamReadCallback(pa_stream* stream, size_t length, void* userdata);

    pa_mainloop* m_mainloop = nullptr;
    pa_context* m_context = nullptr;
    pa_stream* m_stream = nullptr;

    std::thread m_mainloopThread;
    std::atomic<bool> m_ready{false};

    std::mutex m_bufferMutex;
    std::vector<uint8_t> m_audioBuffer;
};

