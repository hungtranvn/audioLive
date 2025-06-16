g++ -std=c++17 -o endianess \
    EndiannessTest.cpp ../src/Endianness.hpp \
    -lgtest -lgtest_main -lgmock -lpthread
