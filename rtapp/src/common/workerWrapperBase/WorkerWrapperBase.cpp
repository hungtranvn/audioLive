#include "WorkerWrapperBase.hpp"

#include <future>
#include <thread>

WorkerWrapperBase::WorkerWrapperBase() : m_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("WRAPPER")))
{

}

WorkerWrapperBase::WorkerWrapperBase(const std::string& loggerName) : m_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(loggerName)))
{

}

auto WorkerWrapperBase::setup() -> void
{
    for (IWorker& worker : m_workerVector) {
        worker.setup();
    }
    logInfo(LogLevel::INFO, m_logger, __func__, name() + " is setup");
}

auto WorkerWrapperBase::run() -> bool
{
    std::vector<std::future<bool>> workerfutureVector{};

    if (m_isEnable.load()) {
        for (IWorker& worker : m_workerVector) {
            workerfutureVector.push_back(std::async(&IWorker::run, &worker));
        }
    }

    const int sleep100Ms = 100;
    logInfo(LogLevel::INFO, m_logger, __func__, name() + " run started");

    while (m_isEnable.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep100Ms));
    }

    bool success = true;
    size_t index = 0U;
    const int sleep400Ms = 400;

    for (std::future<bool>& workerFuture : workerfutureVector) {
        if (workerFuture.wait_for(std::chrono::milliseconds(sleep400Ms)) != 
                std::future_status::ready) {
            logInfo(LogLevel::ERROR, m_logger, __func__, name() + " : " + 
                    m_workerVector.at(index).get().name() + " is not ready");
            success = false;
        } else {
            if (!workerFuture.get()) {
                logInfo(LogLevel::ERROR, m_logger, __func__, name() + " : " +
                        m_workerVector.at(index).get().name() + " run failed");
                success = false;
            }
        }
        ++index;
    }
    
    if (success) {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + " run successfully terminated");
    } else {
        logInfo(LogLevel::INFO, m_logger, __func__, name() + " run terminated with failed");
    }

    return success;
}

auto WorkerWrapperBase::enable(const bool isEnabled) -> void
{
    m_isEnable.store(isEnabled);
    for (IWorker& worker : m_workerVector) {
        worker.enable(isEnabled);
    }
}

auto WorkerWrapperBase::registerWorker(IWorker& worker) -> void 
{
    m_workerVector.push_back(worker);
    logInfo(LogLevel::INFO, m_logger, __func__, name() + " registerWorker " + worker.name());
}
