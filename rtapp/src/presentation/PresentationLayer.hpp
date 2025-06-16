#ifndef PRESENTATION_LAYER_HPP
#define PRESENTATION_LAYER_HPP

#include "common/workerWrapperBase/WorkerWrapperBase.hpp"

#include <log4cplus/logger.h>
#include <atomic>

class PresentationLayer : public WorkerWrapperBase {
public:
    PresentationLayer();
    virtual ~PresentationLayer() = default;
    auto name() const -> const std::string override
    {
        return "PresentationLayer";
    }

    auto registerWorkerWrapper(IWorker& workerWrapper) -> void;

private:
    log4cplus::Logger m_logger;
};

#endif // PRESENTATION_LAYER_HPP
