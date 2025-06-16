g++ -std=c++17 -o waveutils \
    WaveUtilsTest.cpp ../src/WaveUtils.hpp \
    -lgtest -lgtest_main -lgmock -lpthread
