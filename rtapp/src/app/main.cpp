#include "RealtimeApplication.hpp"

#include "utils/Logger.hpp"
#include "utils/Version.hpp"

#include <log4cplus/initializer.h>
#include <log4cplus/configurator.h>
#include <thread>
#include <iostream>

auto mainFunction(const int argc, char const* const* const argv) -> int
{
    try {
        const log4cplus::Initializer initializer;
        log4cplus::BasicConfigurator::doConfigure();
        const log4cplus::Logger mainLog = log4cplus::Logger::getRoot();
        printVersion();
        const std::shared_ptr<RealtimeApplication> rtapp = std::make_shared<RealtimeApplication>();
        rtapp->enable(false);
        rtapp->run();
    } catch (const std::runtime_error& e) {
        const log4cplus::Logger mainLog = log4cplus::Logger::getRoot();
        logInfo(LogLevel::ERROR, mainLog, __func__, "runtime_error: " + std::string(e.what()));
    } catch (const std::logic_error& e) {
        const log4cplus::Logger mainLog = log4cplus::Logger::getRoot();
        logInfo(LogLevel::ERROR, mainLog, __func__, "logic_error: " + std::string(e.what()));
    } catch (const std::bad_alloc& e) {
        const log4cplus::Logger mainLog = log4cplus::Logger::getRoot();
        logInfo(LogLevel::ERROR, mainLog, __func__, "bad_alloc: " + std::string(e.what()));
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        const log4cplus::Logger mainLog = log4cplus::Logger::getRoot();
        logInfo(LogLevel::ERROR, mainLog, __func__, "message: " + std::string(e.what()));
    } catch (...) {
        std::cerr << "Realtime main Unknow exception" << "\n";
        const log4cplus::Logger mainLog = log4cplus::Logger::getRoot(); 
        logInfo(LogLevel::ERROR, mainLog, __func__, "msg: Unknown exception. Call std::terminate().");
    }

    return 0;
}

auto main(const int argc, char const* const* const argv) -> int
{
    return mainFunction(argc, argv);
}
