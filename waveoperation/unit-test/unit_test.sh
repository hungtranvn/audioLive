#g++ -std=c++17 -o waveutils \
#    WaveUtilsTest.cpp ../src/WaveUtils.hpp ../src/ReadableWave.hpp\
#    -lgtest -lgtest_main -lgmock -lpthread

#g++ -std=c++17 -o  wavereader WaveReaderTest.cpp ../src/WaveReader.hpp ../src/WaveReader.cpp ../src/WaveWriter.hpp ../src/WaveWriter.cpp ../src/interface/IWaveFile.hpp -lgtest -lgtest_main -lgmock -lpthread

g++ -std=c++17 -o  writer WaveWriterTest.cpp ../src/WaveWriter.hpp ../src/WaveWriter.cpp ../src/interface/IWaveFile.hpp -lgtest -lgtest_main -lgmock -lpthread
