#include "utils/Version.hpp"

#ifndef GIT_COMMIT_HASH_SHORT
#define GIT_COMMIT_HASH_SHORT "unknown"
#endif // GIT_COMMIT_HASH_SHORT

auto printVersion() -> void
{
    const log4cplus::Logger lLogGitVersion = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("VERSION"));
    logInfo(LogLevel::INFO, lLogGitVersion, "", getVersion());
}

auto getVersion() -> std::string
{
    return std::string("rtapp hash version: " + getCommitHash());
}

auto getCommitHash() -> std::string
{
    return GIT_COMMIT_HASH_SHORT;
}
