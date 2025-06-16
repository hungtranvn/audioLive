#include "WorkerBase.hpp"

WorkerBase::WorkerBase()
    : m_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("WOBA")))
{

}

WorkerBase::WorkerBase(const std::string& loggerName)
    : m_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(loggerName)))
{

}

auto WorkerBase::preRun() -> bool
{
    return true;
}

auto WorkerBase::run() -> bool
{
    bool success = true;

    success = preRun();
    if(success) {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + "preRun succesfully");
    } else {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + "preRun failed");
    }

    while ((m_isEnable.load()) && success) {
        try {
            success = runCustom();
        } catch (const std::exception& e) {
            success = false;
            logInfo(LogLevel::ERROR, m_logger, __func__, "WorkerBase exception for: " + name() + " while running runCustom(): " + std::string(e.what()));
        }
    }

    if(success) {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + " runCustom succesfully");
    } else {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + " runCustom failed");
    }

    if (success) {
        success = postRun();
    }

    if (success) {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + " postRun succesfully");
    } else {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + " postRun failed");
    }

    return success;
}

auto WorkerBase::postRun() -> bool
{
    return true;
}

auto WorkerBase::enable(const bool isEnabled) -> void
{
    m_isEnable.store(isEnabled);
}
