#ifndef VERSION_HPP
#define VERSION_HPP
#include "utils/Logger.hpp"
#include <string>

auto printVersion() -> void;
auto getVersion() -> std::string;
auto getCommitHash() -> std::string;

#endif // VERSION_HPP
