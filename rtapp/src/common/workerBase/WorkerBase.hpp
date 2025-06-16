#ifndef WORKERBASE_HPP
#define WORKERBASE_HPP

#include "interface/IWorker.hpp"
#include "utils/Logger.hpp"

/***
 * @class WorkerBase
 * @brief This abstract class is used to implement the basic functionalities of a worker
 * 
 ***/
class WorkerBase : public virtual IWorker {
public:
    /***
     * @brief Default constructor
     */
    WorkerBase();

    /***
     * @brief Constructor
     * @param loggerName logger name
     */
    explicit WorkerBase(const std::string& loggerName);
    
    /***
     * @brief Default destructor
     */
    ~WorkerBase() override = default;

    /***
     * @brief 
     * @see Interface description 
     */
    auto run() -> bool override;

    /*** 
     * @brief This function will be called before the custom function called
    */
    virtual auto preRun() -> bool;

    /*** 
     * @brief Run function for custom run routine
     * @note Has to be implemented by derived class and will be called by run function
    */
   virtual auto runCustom() -> bool = 0;

   /***
    * @brief This function will be called after the custom run funtion
    */
   virtual auto postRun() -> bool;

   /***
    * @brief See interface function description
    */
   auto enable(bool isEnabled) -> void override;

private:
    log4cplus::Logger m_logger{};
    std::atomic_bool m_isEnable{false};
};
#endif // WORKERBASE_HPP
