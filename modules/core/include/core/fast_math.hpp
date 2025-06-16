#ifndef SCALAR_CORE_FAST_MATH_HPP
#define SCALAR_CORE_FAST_MATH_HPP

#include "defs.hpp"
#include <cmath>

S_INLINE int sRound( double value)
{
    return (int)__builtin_lrint(value);
}

S_INLINE int sFloor( double value)
{
    return (int)__builtin_floor(value);
}

S_INLINE int sCeil( float value)
{
    return (int)__builtin_ceil(value);
}

S_INLINE int sCeil( int value)
{
    return value;
}

S_INLINE int sIsNaN( double value)
{
    return __builtin_isnan(value);
}

S_INLINE int sIsInf( double value)
{
    return __builtin_isinf(value);
}
#endif // SCALAR_CORE_FAST_MATH_HPP