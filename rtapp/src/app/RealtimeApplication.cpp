#include "RealtimeApplication.hpp"

#include "utils/Version.hpp"

#include <pthread.h>
#include <chrono>
#include <thread>

RealtimeApplication::RealtimeApplication() : m_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("RTAPP")))
{

}

RealtimeApplication::~RealtimeApplication() = default;

auto RealtimeApplication::enable(const bool isEnabled) -> void
{
    m_isEnabled.store(isEnabled);
}

auto RealtimeApplication::setup() -> void
{
    logInfo(LogLevel::INFO, m_logger, __func__, "RealtimeApp is setup");
}

auto RealtimeApplication::run() -> bool
{
    pthread_setname_np(pthread_self(), "Realtime Application");

    try {
        while (m_isEnabled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            logInfo(LogLevel::INFO, m_logger, __func__, " in main loop");
        }
    } catch (std::runtime_error& e) {
        logInfo(LogLevel::ERROR, m_logger, __func__, "Realtime APP ERROR: " + std::string(e.what()));
        return false;
    }

    return true;
}
