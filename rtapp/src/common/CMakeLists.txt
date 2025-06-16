cmake_minimum_required(VERSION 3.12)

set(TARGET_NAME commonLib)

set(TARGET_SOURCE_FILES
    workerBase/WorkerBase.cpp
    workerWrapperBase/WorkerWrapperBase.cpp
)

add_library(${TARGET_NAME} SHARED ${TARGET_SOURCE_FILES})

target_include_directories(
    ${TARGET_NAME} 
    PUBLIC ./
    PUBLIC ./workerBase/
    PUBLIC ./workerWrapperBase/
    PUBLIC ${CMAKE_SOURCE_DIR}/src/
    PUBLIC ${CMAKE_SOURCE_DIR}/interface/
)

target_link_libraries(
    ${TARGET_NAME}
    PUBLIC log4cplus::log4cplus
    PRIVATE rt
)


install(TARGETS ${TARGET_NAME} LIBRARY)
