set(TARGET_NAME wav)

set(TARGET_SOURCE_FILES
    wav_header.cpp
    wav_file.cpp
)

add_library(${TARGET_NAME} SHARED ${TARGET_SOURCE_FILES})

# Install the shared library into usr/lib folder
install(TARGETS ${TARGET_NAME} LIBRARY)
