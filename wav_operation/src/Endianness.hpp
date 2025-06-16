#ifndef ENDIANNESS_HPP
#define ENDIANNESS_HPP

#include <cstdint>
#include <type_traits>
#include <algorithm>

namespace wave {
inline bool isLittleEndian() {
    uint16_t one = 1;
    return *reinterpret_cast<uint8_t*>(&one) == 1;
}

inline bool isBigEndian() {
    return !isLittleEndian();
}

template <typename T>
void swapEndianness(T& value) {
    static_assert(std::is_trivially_copyable<T>::value, "Type must be trivially copyable");
    auto* ptr = reinterpret_cast<uint8_t*>(&value);
    std::reverse(ptr, ptr + sizeof(T));
}

template <typename T>
void toLittleEndian(T& value) {
    static_assert(std::is_integral<T>::value, "Type must be an integral");
    if (isBigEndian())
        swapEndianness(value);
}

template <typename T>
void toBigEndian(T& value) {
    if (isLittleEndian())
        swapEndianness(value);
}
} // namespace wav
#endif // ENDIANNESS_HPP
