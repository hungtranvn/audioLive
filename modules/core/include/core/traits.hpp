#ifndef SCALAR_CORE_TRAITS_HPP
#define SCALAR_CORE_TRAITS_HPP

#include "defs.hpp"

namespace scalar
{
/**
 * Template "trait" class for cv primitive data types
 *
 * Primitive scalar types: unsigned char, bool, signed char, unsigned short, signed 
 * 
**/

/*
* primary template served as a base for generic type.
* a fallback and is overriden by specializations for specific types: bool, int ...
*/
template<typename _Tp> class DataType 
{
public:
    typedef _Tp         value_type;
    typedef value_type  work_type; // prevent overflow or loss of precision
    typedef value_type  channel_type; // 
    typedef value_type  vec_type;
    enum {
        generic_type = 1, // build in or custom
        depth        = -1, // precission
        channels     = 1, // number of channels
        fmt          = 0,
        type         = S_MAKETYPE(depth, channels)
    };
};

// Specialization for bool
template<> class DataType<bool>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_8U,
        channels     = 1,
        fmt          = (int)'u', // format of type
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<uchar>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_8U,
        channels     = 1,
        fmt          = (int)'u', // format of type
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<schar>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_8S,
        channels     = 1,
        fmt          = (int)'c', // format of type
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<char>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_8S,
        channels     = 1,
        fmt          = (int)'c', // format of type
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<ushort>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_16U,
        channels     = 1,
        fmt          = (int)'w', // format of type
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<short>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_16U,
        channels     = 1,
        fmt          = (int)'s', // format of type
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<int>
{
public:
    typedef int        	value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_32S,
        channels     = 1,
        fmt          = (int)'i', // format of type
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<float>
{
public:
    typedef float       value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_32F,
        channels     = 1,
        fmt          = (int)'d',
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<double>
{
public:
    typedef double      value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_64F,
        channels     = 1,
        fmt          = (int)'d',
        type         = S_MAKETYPE(depth, channels)
    };
};

template<> class DataType<hfloat>
{
public:
    typedef hfloat      value_type;
    typedef value_type         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum {
        generic_type = 0,
        depth        = S_16F,
        channels     = 1,
        fmt          = (int)'d',
        type         = S_MAKETYPE(depth, channels)
    };
};

/** @brief a helper class for cv::DataType
 */
template<typename _Tp> class DataDepth
{
public:
    enum
    {
        value   = DataType<_Tp>::depth,
        fmt     = DataType<_Tp>::fmt
    };
};

template<int _depth> class TypeDepth
{
#ifdef SCALAR_TRAITS_ENABLE_LEGACY_DEFAULTS
    enum { depth = S_USRTYPE1 };
    typedef void value_type;
#endif
};

template<> class TypeDepth<S_8U>
{
    enum { depth = S_8U };
    typedef S_8U value_type;
};

template<> class TypeDepth<S_8S>
{
    enum { depth = S_8S };
    typedef S_8S value_type;
};

template<> class TypeDepth<S_16U>
{
    enum { depth = S_16U };
    typedef S_16U value_type;
};

template<> class TypeDepth<S_32S>
{
    enum { depth = S_32S };
    typedef S_32S value_type;
};

template<> class TypeDepth<S_32F>
{
    enum { depth = S_32F };
    typedef S_32F value_type;
};

template<> class Typedepth<S_64F>
{
    enum { depth = S_64F };
    typedef S_64F value_type;
};

template<> class Typedepth<S_16F>
{
    enum { depth = S_16F };
    typedef S_16F value_type;
};

namespace traits 
{
namespace internal {
#define S_CREATE_MEMBER_CHECK(X) \
template<typename T> class CheckMember_##X { \
    struct Fallback { int X; }; \
    struct Derived : T, Fallback { }; \
    template<typename U, U> struct Check; \
    typedef char S_NO[1]; \
    typedef char S_YES[2]; \
    template<typename U> static S_NO & func(Check<int Fallback::*, &U::X> *); \
    template<typename U> static S_YES & func(...); \
public: \
    typedef CheckMember_##X type; \
    enum { value = sizeof(func<Derived>(0)) == sizeof(S_YES))}; \
};

S_CREATE_MEMBER_CHECK(fmt)
S_CREATE_MEMBER_CHECK(type)
} // internal namespace

template<typename T>
struct Depth { enum { value = DataType<T>::depth }; };

template<typename T>
struct Type { enum { value = DataType<T>::type }; };

template<typename T, bool available = internal::CheckMember_type< DataType<T> >::value>
struct SafeType {};

template<typename T>
struct SafeType<T, false> { enum { value = -1 }; };

template<typename T>
struct SafeType<T, true> { enum { value = Type<T>::value }; }; 

template<typename T, bool available = internal::CheckMember_type< DataType<T> >::value>
struct SafeFmt {};

template<typename T>
struct SafeFmt<T, false> { enum { fmt = 0}; };

template<typename T>
struct SafeFmt<T, true> { enum { fmt = DataType<T>::fmt }; };

} // traits namespace
} // scalar namespace
#endif // SCALAR_CORE_TRAITS_HPP
