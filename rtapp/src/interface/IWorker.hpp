#ifndef IWORKER_HPP
#define IWORKER_HPP

#include <string>
/*** 
 * @brief This interface should be implemented by every class which tends to be run in a thread 
 * 
 ***/ 
class IWorker {
public:
    virtual ~IWorker() { };
    /***
     * @brief This function shall return the name of worker
     * @param None
     * @return Name of worker
     * @note None
     ***/
    virtual auto name() const -> std::string = 0;
    
    /***
     * @brief This function to do initial setup. Should call before running
     * @param None
     * @return None
     * @note Pure virtual function
     ***/
    virtual auto setup() -> void = 0;

    /***
     * @brief This function to be called/ran in separete thread
     * @param None
     * @return boolean
     * @note While implementing this interface, everything in this function should be thread safe
     ***/
    virtual auto run() -> bool = 0;

    /***
     * @brief This function designed to enable/disable a worker
     * @param None
     * @return boolean
     * @note While implementing this interface, this function end/disable a running run() function. Need to check enable state regularly
     ***/
    virtual auto enable(bool isEnabled) -> void = 0;
};
#endif // IWORKER_HPP