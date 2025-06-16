#include "PAProxy.hpp"
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    PAProxy proxy;
    if (!proxy.initialize("RDPSink.monitor")) {
        std::cerr << "Failed to initialize PAProxy\n";
        return 1;
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto data = proxy.fetchLatestSamples();
        std::cout << "Fetched buffer size: " << data.size() << " bytes\n";
    }

    proxy.shutdown();
    return 0;
}

