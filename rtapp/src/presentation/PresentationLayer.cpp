#include "PresentationLayer.hpp"

#include <log4cplus/logingmacros.h>

#include <chrono>
#include <future>

PresentationLayer::PresentationLayer() : WorkerWrapperBase("presentation"),
    m_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("presentation")))
{
}

auto PresentationLayer::registerWorkerWrapper(IWorker workerWrapper) -> void
{
    registerWorker(workerWrapper);
}



