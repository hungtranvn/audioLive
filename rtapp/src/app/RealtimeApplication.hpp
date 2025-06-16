#ifndef REALTIMEAPP_HPP
#define REALTIMEAPP_HPP

#include "utils/Logger.hpp"
#include "interface/IWorker.hpp"

#include <memory>
#include <optional>

class RealtimeApplication : public IWorker {
public:
    RealtimeApplication();
    ~RealtimeApplication() override;

    auto name() const -> std::string override
    {
        return "RealtimeApplication";
    }
    
    auto enable(bool isEnabled) -> void override;
    auto setup() -> void override;
    auto run() -> bool override;
private:
    std::atomic_bool m_isEnabled{false};
    log4cplus::Logger m_logger;
};
#endif //REALTIMEAPP_HPP
