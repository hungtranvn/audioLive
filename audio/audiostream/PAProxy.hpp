#ifndef PA_PROXY_HPP
#define PA_PROXY_HPP

#include <pulse/pulseaudio.h>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
#include <thread>

namespace audio {
class PAProxy {
public:
    //PAProxy();
    virtual ~PAProxy() = default;
    /*
    bool initialize(const std::string& sourceName);
    void shutdown();
    std::vector<uint8_t> fetchLatestSamples();
    */
    // property list
    virtual auto proplistNew() -> pa_proplist* = 0;
    virtual auto proplistSets(pa_proplist* pProplist, const char* key, const char* value) -> int = 0;
    virtual auto proplistToStringSep(const pa_proplist* pProplist, const char* sep) -> char* = 0;
    virtual auto proplistFree(pa_proplist* pProplist);
    
    // context
    virtual auto contextNew(pa_mainloop_api* mainloop, const char* name) -> pa_context* = 0;
    virtual void contextUnref(pa_context* pContext);
    virtual void contextSetStateCallback(pa_context* pContext, pa_context_notify_cb_t callback, void* userdata) = 0;
    virtual auto contextErrno(const pa_context* pContext) -> int = 0;
    virtual auto contextGetState(const pa_context* pContext) -> int = 0;
    virtual auto contextGetState(const pa_context* pContext) -> pa_context_state_t = 0;
    virtual auto contextConnect(pa_context* pContext, const char* server, pa_context_flags_t flags, const pa_spawn_api* api) -> int = 0;
    virtual auto contextRttineRestart(const pa_context* pContext, pa_time_event* event, pa_usec_t usec) -> void = 0;
    virtual contextDisconnect(pa_context* pContext) -> void = 0;
    virtual contextSetSubcribeCallback(pa_context* pContext, pa_context_subscribe_ab_t callback, void* userdata) -> void = 0;
    virtual auto contextSubcribe(pa_context* pContext, pa_subscription_mask_t mask, pa_context_success_cb_t callback, void* userdata) -> pa_operation* = 0;
    virtual auto contextGetSinkInfoList(pa_context* pContext, pa_sink_info_cb_t callback, void* userdata) -> pa_operation* = 0;
    virtua; auto contextGetSinkInfoByName(pa_context* pContext, const char* name, pa_sink_info_cb_t callback, void* userdata) -> pa_operation* = 0;
    
    // stream
    virtual auto streamNewWithProplist(pa_context* pContext, const char* name, const pa_sample_spec* sampleSpec, const pa_channel_map* map, pa_proplist* pProplist) -> pa_stream* = 0;
    virtual streamSetStateCallback(pa_stream* stream, pa_stream_notify_cb_t callback, void* userdata) -> void = 0;
    virtual streamSetReadCallback(pa_stream* pStream, pa_stream_request_cb_t callback, void* userdata) -> void = 0;
    virtual streamSetOverflowCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void = 0;
    virtual streamSetEventCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void = 0;
    virtual streamSetStartedCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void = 0;
    virtual streamSetSuspendedCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void = 0;
    virtual auto streamConnectRecord(pa_stream* pStream, const char* dev, const pa_buffer_attr* attr, pa_stream_flags_t flags) -> int = 0;
    virtual auto streamGetState(const pa_stream* pStream) -> pa_stream_state_t = 0;
    virtual auto streamCork(pa_stream* pStream, int bFlag, pa_stream_success_cb_t callback, void* nbytes) -> int = 0;
    virtual auto streamPeek(pa_stream* pStream, const void** data, size_t* nbytes) -> int = 0;
    virtual auto streamDrop(pa_stream* pStream) -> int = 0;
    virtual auto streamIsSuspended(const pa_stream* stream) -> int = 0;
    virtual auto streamUnref(pa_stream* stream) -> void = 0;
    virtual auto operationUnref(pa_stream* operation) -> void = 0;
/*
private:
    static void streamReadCallback(pa_stream* stream, size_t length, void* userdata);

    pa_mainloop* m_mainloop = nullptr;
    pa_context* m_context = nullptr;
    pa_stream* m_stream = nullptr;

    std::thread m_mainloopThread;
    std::atomic<bool> m_ready{false};

    std::mutex m_bufferMutex;
    std::vector<uint8_t> m_audioBuffer;
*/
};
} // namespace audio
#endif // PA_PROXY_HPP
