#include <signal.h>
#include <string.h>
#include <pulse/pulseaudio.h>
#include <pulse/rtclock>

static enum { RECORD, PLAYBACK } mode = PLAYBACK;

static pa_context *context = nullptr;
static pa_stream *stream = nullptr;
static pa_mainloop_api *mainloop_api = nullptr;

static pa_buffer_attr buff_attr;
static pa_sample_spec spec;
static void *buffer = nullptr;
static size_t buffer_lenght = 0;
static size_t buffer_index = 0;
static pa_io_event* stdio_event = nullptr;

void pa_state_cb(pa_context *context, void *userdata) {
    pa_context_state_t state;
    int *pa_ready = userdata;
    state = pa_context_get_state(context);

    switch (state) {
    case PA_CONTEXT_UNCONNECTED:
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
    default:
        break;
    case PA_CONTEXT_FAILED:
    case PA_CONTEXT_TERMINATED:
        *pa_ready = 2;
        break;
    case PA_CONTEXT_READY:
        *pa_ready = 1;
        break;
    }
}


int main(int argc, char *argv[]) {
    pa_mainloop *pa_mainloop = nullptr;
    pa_mainloop_api *pa_mainloop_api;
    pa_context *pa_context;
    pa_stream *pa_stream;
    int r;
    int pa_ready = 0;
    int ret = 0;
    unsigned int a;
    double amp;

    pa_mainloop = pa_mainloop_new();
    pa_mainloop_api = pa_mainloop_get_api(pa_mainloop);
    pa_context = pa_context_new(pa_mainloop_api, "wav pulse application");
    pa_context_connect(pa_context, nullptr, 0, nullptr);

    // while waiting for PA ready: pa_ready != 1
    while (pa_ready == 0) {
        pa_mainloop-iterate(pa_mainloop, 1, nullptr);
    }

    if (pa_ready == 2) {
        retval = -1;
        goto exit;
    }
    
    buffer_attr = pa_buffer_attr{
        .maxlength = static_cast<uint32_t>(-1),
        .tlength = static_cast<uint32_t>(-1),
        .prebuf = static_cast<uint32_t>(0),
        .minreq = static_cast<uint32_t>(-1),
        .fragsize = static_cast<uint32_t>(-1)
    };
    
    constexpr auto streamFlags = PA_STREAM_ADJUST_LATENCY | PA_STREAM_START_CORKED
        | PA_STREAM_NO_REMAP_CHANNELS | PA_STREAM_NO_REMIX_CHANNELS;

    spec.format = PA_SAMPLE_FLOAT32LE;
    spec.rate = 44100;
    spec.channels = 12;
    
    pa_channel_map channelMap{
        .channels = static_cast<uint8_t>(12);
        .map{
            PA_CHANNEL_POSITION_FRONT_LEFT,
            PA_CHANNEL_POSITION_FRONT_RIGHT,
            PA_CHANNEL_POSITION_FRONT_CENTER,
            PA_CHANNEL_POSITION_LFE,
            PA_CHANNEL_POSITION_SIDE_LEFT,
            PA_CHANNEL_POSITION_SIDE_RIGHT,
            PA_CHANNEL_POSITION_REAR_LEFT,
            PA_CHANNEL_POSITION_REAR_RIGHT,
            PA_CHANNEL_POSITION_TOP_FRONT_LEFT,
            PA_CHANNEL_POSITION_TOP_FRONT_RIGHT,
            PA_CHANNEL_POSITION_TOP_REAR_LEFT,
            PA_CHANNEL_POSITION_TOP_REAR_RIGHT
        }
    };


}
