#ifndef PA_PROXY_IMPL_HPP
#define PA_PROXY_IMPL_HPP

#include "PAProxy.hpp"

namespace audio {
class PAProxyImpl {
public:
    virtual ~PAProxy() = default;
    /*
    bool initialize(const std::string& sourceName);
    void shutdown();
    std::vector<uint8_t> fetchLatestSamples();
    */
    // property list
    virtual auto proplistNew() -> pa_proplist* override;
    virtual auto proplistSets(pa_proplist* pProplist, const char* key, const char* value) -> int override;
    virtual auto proplistToStringSep(const pa_proplist* pProplist, const char* sep) -> char* override;
    virtual auto proplistFree(pa_proplist* pProplist) -> void override;
    
    // context
    virtual auto contextNew(pa_mainloop_api* mainloop, const char* name) -> pa_context* override;
    virtual contextUnref(pa_context* pContext) -> void override;
    virtual contextSetStateCallback(pa_context* pContext, pa_context_notify_cb_t callback, void* userdata) -> void override;
    virtual auto contextErrno(const pa_context* pContext) -> int override;
    virtual auto contextGetState(const pa_context* pContext) -> int override;
    virtual auto contextGetState(const pa_context* pContext) -> pa_context_state_t override;
    virtual auto contextConnect(pa_context* pContext, const char* server, pa_context_flags_t flags, const pa_spawn_api* api) -> int override;
    virtual auto contextRttineRestart(const pa_context* pContext, pa_time_event* event, pa_usec_t usec) -> void override;
    virtual contextDisconnect(pa_context* pContext) -> void override;
    virtual contextSetSubcribeCallback(pa_context* pContext, pa_context_subscribe_ab_t callback, void* userdata) -> void override;
    virtual auto contextSubcribe(pa_context* pContext, pa_subscription_mask_t mask, pa_context_success_cb_t callback, void* userdata) -> pa_operation* override;
    virtual auto contextGetSinkInfoList(pa_context* pContext, pa_sink_info_cb_t callback, void* userdata) -> pa_operation* override;
    virtua; auto contextGetSinkInfoByName(pa_context* pContext, const char* name, pa_sink_info_cb_t callback, void* userdata) -> pa_operation* override;
    
    // stream
    virtual auto streamNewWithProplist(pa_context* pContext, const char* name, const pa_sample_spec* sampleSpec, const pa_channel_map* map, pa_proplist* pProplist) -> pa_stream* override;
    virtual streamSetStateCallback(pa_stream* stream, pa_stream_notify_cb_t callback, void* userdata) -> void override;
    virtual streamSetReadCallback(pa_stream* pStream, pa_stream_request_cb_t callback, void* userdata) -> void override;
    virtual streamSetOverflowCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void override;
    virtual streamSetEventCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void override;
    virtual streamSetStartedCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void override;
    virtual streamSetSuspendedCallback(pa_stream* pStream, pa_stream_notify_cb_t callback, void* userdata) -> void override;
    virtual auto streamConnectRecord(pa_stream* pStream, const char* dev, const pa_buffer_attr* attr, pa_stream_flags_t flags) -> int override;
    virtual auto streamGetState(const pa_stream* pStream) -> pa_stream_state_t override;
    virtual auto streamCork(pa_stream* pStream, int bFlag, pa_stream_success_cb_t callback, void* nbytes) -> int override;
    virtual auto streamPeek(pa_stream* pStream, const void** data, size_t* nbytes) -> int override;
    virtual auto streamDrop(pa_stream* pStream) -> int override;
    virtual auto streamIsSuspended(const pa_stream* stream) -> int override;
    virtual auto streamUnref(pa_stream* stream) -> void override;
    virtual auto  operationUnref(pa_stream* operation) -> void override;
};
} // namepsace audio
#endif // PA_PROXY_IMPL_HPP
