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
void swapEndian(T& value) {
    static_assert(std::is_trivially_copyable<T>::value, "Type must be trivially copyable");
    auto* ptr = reinterpret_cast<uint8_t*>(&value);
    std::reverse(ptr, ptr + sizeof(T));
}

template <typename T>
T toLittleEndian(T value) {
    static_assert(std::is_integral<T>::value, "Type must be an integral");
    T tmp = value;
    if (isBigEndian())
        swapEndian(tmp);
    return tmp;
}

template <typename T>
T toBigEndian(T value) {
    T tmp = value;
    if (isLittleEndian())
        swapEndian(tmp);
    return tmp;
}
} // namespace wav
#endif // ENDIANNESS_HPP
