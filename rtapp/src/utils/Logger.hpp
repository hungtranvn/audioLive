#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include <string>

enum class LogLevel : int { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

inline void logInfo(
    const LogLevel level,
    const log4cplus::Logger& logger,
    const std::string& func,
    const std::string& message)
{
    switch (level) {
    case LogLevel::TRACE:
        LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT(func << ":" << message));
        break;
    case LogLevel::DEBUG:
        LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT(func << ":" << message));
        break;
    case LogLevel::INFO:
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT(func << ":" << message));
        break;
    case LogLevel::WARN:
        LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT(func << ":" << message));
        break;
    case LogLevel::ERROR:
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT(func << ":" << message));
        break;
    case LogLevel::FATAL:
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT(func << ":" << message));
        break;
    default:
        break;
    }
}
#endif // LOGGER_HPP
