#include "PAProxyImplImpl.hpp"
#include <iostream>
#include <cstring>
#include <thread>

namespace audio {
PAProxyImpl::PAProxyImpl() = default;
PAProxyImpl::~PAProxyImpl() { shutdown(); }

// property list
auto PAProxyImpl::proplistNew() -> pa_proplist*
{
    return pa_proplist_new();
}

auto PAProxyImpl::proplistSets(pa_proplist* pProplist, 
        const char* key, const char* value) -> int
{
    return pa_proplist_sets(pProplist, key, value);
}

auto PAProxyImpl::proplistToStringSep(const pa_proplist* pProplist, 
        const char* sep) -> char*
{
    return pa_proplist_to_string_sep(pProplist, sep);
}

auto PAProxyImpl::proplistFree(pa_proplist* pProplist) -> void
{
    return pa_proplist_free(pProplist);
}

// context
auto PAProxyImpl::contextNew(pa_mainloop_api* mainloop, 
        const char* name) -> pa_context*
{
    return pa_context_new(mainloop, name);
}

auto PAProxyImpl::contextUnref(pa_context* pContext) -> void
{
    return pa_context_unref(pContext);
}

auto PAProxyImpl::contextSetStateCallback(pa_context* pContext, 
        pa_context_notify_cb_t callback, void* userdata) -> void
{
    return pa_context_set_state_callback(pContext, callback, userdata);
}

auto PAProxyImpl::contextErrno(const pa_context* pContext) -> pa_context_state_t;
{
    return pa_context_error(pContext);
}

auto PAProxyImpl::contextGetState(const pa_context* pContext) -> int
{
    return pa_context_get_state(pContext);
}

auto PAProxyImpl::contextConnect(pa_context* pContext, const char* server,
        pa_context_flags_t flags, const pa_spawn_api* api) -> int
{
    return pa_context_connect(pContext, server, flags, api);
}

auto PAProxyImpl::contextRttimeRestart(const pa_context* pContext, pa_time_event* event,
        pa_usec_t usec) -> void
{
    return pa_context_rttime_restart(pContext, event, usec);
}

auto PAProxyImpl::contextDisconnect(pa_context* pContext) -> void
{
    return pa_context_disconnect(pContext);
}

auto PAProxyImpl::contextSetSubscribeCallback(pa_context* pContext, 
        pa_context_subscribe_cb_t callback, void* userdata) -> void
{
    return pa_context_set_subscribe_callback(pContext, callback, userdata);
}

auto PAProxyImpl::contextSubscribe(pa_context* pContext, pa_subscription_mask_t mask,
        pa_context_success_cb_t callback,
        void* userdata) -> pa_operation*
{
    return pa_context_subscribe(pContext, mask, callback, userdata);
}

auto PAProxyImpl::contextGetSinkInfoList(pa_context* pContext, pa_sink_info_cb_t callback,
        void* userdata) -> pa_operation*
{
    return pa_context_get_sink_info_list(pContext, callback, userdata);
}

auto PAProxyImpl::contextGetSinkInfoByName(pa_context* pContext, const char* name,
        pa_sink_info_cb_t callback, void* userdata) -> pa_operation*
{
    return pa_context_get_sink_info_by_name(pContext, name, callback, userdata);
}

// stream
auto PAProxyImpl::streamNewWithProplist(pa_context* pContext, const char* name,
        const pa_sample_spec* sampleSpec, const pa_channel_map* map, pa_proplist* pProplist) -> pa_stream*
{
    return pa_stream_set_state_callback(stream, callback, userdata);
}

auto PAProxyImpl::streamSetStateCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void
{
    return pa_stream_set_state_callback(pStream, callback, userdata);
}

auto PAProxyImpl::streamSetReadCallback(pa_stream* pStream, pa_stream_request_cb_t callback, void* userdata) -> void
{
    return pa_stream_set_read_callback(pStream, callback, userdata);
}

auto PAProxyImpl::streamSetOverflowCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void
{
    return pa_stream_set_overflow_callback(pStream, callback, userdata);
}

auto PAProxyImpl::streamSetEventCallback(pa_stream* pStream, pa_stream_event_cb_t callback, void* userdata) -> void
{
    return pa_stream_set_event_callback(pStream, callback, userdata);
}

auto PAProxyImpl::streamSetStartedCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void
{
    return pa_stream_set_started_callback(pStream, callback, userdata);
}

auto PAProxyImpl::streamSetSuspendedCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void
{
    return pa_stream_set_suspended_callback(pStream, callback, userdata);
}

auto PAProxyImpl::streamConnectRecord(pa_stream* pStream, const char* dev, const pa_buffer_attr* attr, pa_stream_flags_t flags) -> int
{
    return pa_stream_connect_record(pStream, dev, attr, flags);
}

auto PAProxyImpl::streamGetState(const pa_stream* pStream) -> pa_stream_state_t
{
    return pa_stream_get_state(pStream);
}

auto PAProxyImpl::streamCork(pa_stream* pStream, int bFlag, pa_stream_success_cb_t callback, void* userdata) -> int
{
    return pa_stream_cork(pStream, bFlag, callback, userdata);
}

auto PAProxyImpl::streamPeek(pa_stream* pStream, const void** data, size_t* nbytes*) -> int
{
    return pa_stream_peek(pStream, data, nbytes);
}

auto PAProxyImpl::streamDrop(pa_stream* pStream) -> int
{
    return pa_stream_drop(pStream);
}

auto PAProxyImpl::streamIsSuspended(const pa_stream* stream) -> int
{
    return pa_stream_is_suspended(stream);
}

auto PAProxyImpl::streamUnref(pa_stream* stream) -> void
{
    return pa_stream_unref(stream);
}

auto PAProxyImpl::operationUnref(pa_operation* operation) -> void
{
    return pa_operation_unref(operation);
}

auto

auto PAProxyImpl::initialize(const std::string& sourceName) -> bool
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
        auto* proxy = static_cast<PAProxyImpl*>(userdata);
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

    pa_stream_set_read_callback(m_stream, &PAProxyImpl::streamReadCallback, this);

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

void PAProxyImpl::shutdown() {
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

void PAProxyImpl::streamReadCallback(pa_stream* stream, size_t, void* userdata) {
    auto* proxy = static_cast<PAProxyImpl*>(userdata);
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

std::vector<uint8_t> PAProxyImpl::fetchLatestSamples() {
    std::lock_guard<std::mutex> lock(m_bufferMutex);
    std::cout << "size of data" << m_audioBuffer.size() << std::endl;
    return m_audioBuffer;
}
}
