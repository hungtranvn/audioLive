#include "PAProxy.hpp"
#include <iostream>
#include <cstring>
#include <thread>

PAProxy::PAProxy() = default;
PAProxy::~PAProxy() { shutdown(); }

auto PAProxy::initialize(const std::string& sourceName) -> bool
{
    m_mainloop = pa_mainloop_new();
    if (!m_mainloop) {
        std::cerr << "Failed to create mainloop\n";
        return false;
    }

    m_context = pa_context_new(pa_mainloop_get_api(m_mainloop), "AudioProxy");
    if (!m_context) {
        std::cerr << "Failed to create context\n";
        return false;
    }

    pa_context_set_state_callback(m_context, [](pa_context* c, void* userdata) {
        auto* proxy = static_cast<PAProxy*>(userdata);
        pa_context_state_t state = pa_context_get_state(c);

        std::cout << "Context state: " << state << "\n";
        if (state == PA_CONTEXT_READY) {
            proxy->m_ready.store(true);
        } else if (state == PA_CONTEXT_FAILED || state == PA_CONTEXT_TERMINATED) {
            proxy->m_ready.store(true);
            std::cerr << "Context failed or terminated\n";
        }
    }, this);

    if (pa_context_connect(m_context, nullptr, PA_CONTEXT_NOFLAGS, nullptr) < 0) {
        std::cerr << "Failed to connect context\n";
        return false;
    }

    // Wait until context is ready or failed
    while (!m_ready.load()) {
        pa_mainloop_iterate(m_mainloop, 1, nullptr);
    }

    if (pa_context_get_state(m_context) != PA_CONTEXT_READY) {
        std::cerr << "Context not ready\n";
        return false;
    }

    pa_sample_spec sampleSpec {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };

    m_stream = pa_stream_new(m_context, "Capture", &sampleSpec, nullptr);
    if (!m_stream) {
        std::cerr << "Failed to create stream\n";
        return false;
    }

    pa_stream_set_read_callback(m_stream, &PAProxy::streamReadCallback, this);

    if (pa_stream_connect_record(m_stream, sourceName.c_str(), nullptr, PA_STREAM_NOFLAGS) < 0) {
        std::cerr << "Failed to connect record stream\n";
        return false;
    }

    // Wait for stream to be ready
    while (pa_stream_get_state(m_stream) != PA_STREAM_READY) {
        pa_mainloop_iterate(m_mainloop, 1, nullptr);
    }

    std::cout << "Stream ready. Starting mainloop...\n";

    // Run mainloop in a background thread
    m_mainloopThread = std::thread([this]() {
        int ret = 0;
        pa_mainloop_run(m_mainloop, &ret);
    });

    return true;
}

void PAProxy::shutdown() {
    if (m_mainloop) {
        pa_mainloop_quit(m_mainloop, 0);
        if (m_mainloopThread.joinable()) {
            m_mainloopThread.join();
        }
    }

    if (m_stream) pa_stream_unref(m_stream);
    if (m_context) pa_context_unref(m_context);
    if (m_mainloop) pa_mainloop_free(m_mainloop);

    m_stream = nullptr;
    m_context = nullptr;
    m_mainloop = nullptr;
}

void PAProxy::streamReadCallback(pa_stream* stream, size_t, void* userdata) {
    auto* proxy = static_cast<PAProxy*>(userdata);
    const void* data = nullptr;
    size_t nbytes = 0;

    if (pa_stream_peek(stream, &data, &nbytes) < 0 || !data || nbytes == 0)
        return;

    {
        std::lock_guard<std::mutex> lock(proxy->m_bufferMutex);
        proxy->m_audioBuffer.assign(static_cast<const uint8_t*>(data), static_cast<const uint8_t*>(data) + nbytes);
    }

    std::cout << "Received " << nbytes << " bytes of audio\n";
    pa_stream_drop(stream);
}

std::vector<uint8_t> PAProxy::fetchLatestSamples() {
    std::lock_guard<std::mutex> lock(m_bufferMutex);
    std::cout << "size of data" << m_audioBuffer.size() << std::endl;
    return m_audioBuffer;
}

