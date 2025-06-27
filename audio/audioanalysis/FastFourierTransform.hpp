#ifndef FAST_FOURIER_TRANSFORM_HPP
#define FAST_FOURIER_TRANSFORM_HPP

#include <complex>
#include <vector>
#include <cmath>

namespace fft {
auto processFft(std::vector<std::complex<float>>& data, unsigned int logOfBufferSizeBase2, bool forward = true) -> void
{
    constexpr float twoPointZeroFloat = 2.0F;
    auto fftSize = (unsigned int)data.size();

    //fftSize/2
    const unsigned int ii2 = fftSize >> 1;
}
} // namespace fft
#endif // FAST_FOURIER_TRANSFORM_HPP
