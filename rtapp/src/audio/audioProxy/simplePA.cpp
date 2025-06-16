#include <pulse/simple.h>
#include <pulse/error.h>
#include <iostream>
#include <cstring>

int main() {
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };

    int error;
    pa_simple *s = pa_simple_new(
        nullptr,               // Use default server
        "TestApp",             // Our application's name
        PA_STREAM_RECORD,
        "RDPSink.monitor",     // Source name
        "record",              // Stream description
        &ss,                   // Sample format
        nullptr,               // Use default channel map
        nullptr,               // Use default buffering
        &error
    );

    if (!s) {
        std::cerr << "pa_simple_new() failed: " << pa_strerror(error) << std::endl;
        return 1;
    }

    std::cout << "Recording from RDPSink.monitor...\n";

    for (int i = 0; i < 10; ++i) {
        uint8_t buffer[1024];
        if (pa_simple_read(s, buffer, sizeof(buffer), &error) < 0) {
            std::cerr << "pa_simple_read() failed: " << pa_strerror(error) << std::endl;
            pa_simple_free(s);
            return 1;
        }

        std::cout << "Read " << sizeof(buffer) << " bytes of audio data.\n";
    }

    pa_simple_free(s);
    return 0;
}
