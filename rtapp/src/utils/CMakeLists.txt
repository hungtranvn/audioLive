set(TARGET_NAME utilsLib)

set(TARGET_SOURCE_FILES Logger.hpp Version.cpp)

find_package(Boost)

add_library(${TARGET_NAME} SHARED ${TARGET_SOURCE_FILES})

target_include_directories(
    ${TARGET_NAME}
    PUBLIC ./
    PUBLIC ${CMAKE_SOURCE_DIR}/src/
    PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME} PUBLIC ${Boost_LIBRARIES})

execute_process(
    COMMAND git config --global --add safe.directory ${CMAKE_SOURCE_DIR} OUTPUT_QUIET ERROR_QUIET
)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions(
    ${TARGET_NAME} PRIVATE "-DGIT_COMMIT_HASH_SHORT=\"${GIT_COMMIT_HASH_SHORT}\""
)

install(TARGETS ${TARGET_NAME} LIBRARY)

