g++ -std=c++17 -o waveutils \
    WaveUtilsTest.cpp ../src/WaveUtils.hpp ../src/ReadableWave.hpp\
    -lgtest -lgtest_main -lgmock -lpthread
