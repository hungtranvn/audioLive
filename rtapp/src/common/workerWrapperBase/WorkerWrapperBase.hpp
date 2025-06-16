#ifndef WORKERWRAPPERBASE_HPP
#define WORKERWRAPPERBASE_HPP

#include "interface/IWorker.hpp"
#include "utils/Logger.hpp"

#include <vector>

class WorkerWrapperBase : public virtual IWorker {
public:
    /***
     * @brief Default constructor
     */
    WorkerWrapperBase();

    /***
     * @brief Constructor
     * @param loggerName 
     */
    explicit WorkerWrapperBase(const std::string& loggerName);

    /***
     * @brief Destructor
     */
    ~WorkerWrapperBase() override = default;

    /***
     * @brief setup and initialize all workers
     */
    auto setup() -> void override;

    /***
     * @brief run all workers
     */
    auto run() -> bool override;

    /***
     * @brief enable/disable all workers
     */
    auto enable(bool isEnabled) -> void override;

    /***
     * @brief register a worker which be handeled by the workerwrapper
     * @param worker
     */
    auto registerWorker(IWorker& worker) -> void;

private:
    log4cplus::Logger m_logger{};
    std::vector<std::reference_wrapper<IWorker>> m_workerVector{};
    std::atomic_bool m_isEnable{false};
};
#endif // WORKERWRAPPERBASE_HPP
