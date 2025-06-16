cmake_minimum_required(VERSION 3.12)

# Setthe project name
project(RealtimeService)

# Add executable
add_executable(realtimeService main.cpp RealtimeApplication.cpp)

# Link necessary libraries
target_link_libraries(
    realtimeService
    PRIVATE log4cplus::log4cplus
    PRIVATE commonLib
    PRIVATE utilsLib
)

# Specify include directories
target_include_directories(
    realtimeService
    PUBLIC /usr/local/include        # Custom include path for log4cplus
    PUBLIC ${CMAKE_SOURCE_DIR}/src/  # Project include path
    PUBLIC ./                        # Current directory (optional)
)

# Specify library directories
target_link_directories(
    realtimeService
    PUBLIC /usr/local/lib            # Custom library path for log4cplus
)

# Installation instructions
install(TARGETS realtimeService DESTINATION ${CMAKE_INSTALL_BINDIR})
