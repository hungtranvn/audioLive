#ifndef S_DEFS_HPP
#define S_DEFS_HPP

#include <stdint.h>

typedef unsigned char uchar;
typedef          char schar;

typedef unsigned short ushort;
typedef int64_t int64;
typedef uint64_t uint64;

#define S_CN_MAX       512
#define S_CN_SHIFT     3
#define S_DEPTH_MAX    (1 << S_CN_SHIFT)

#define S_8U    0
#define S_8S    1
#define S_16U   2
#define S_16S   3
#define S_32S   4
#define S_32F   5
#define S_64F   6
#define S_16F   7

#define S_MAT_DEPTH_MAKS    (S_DEPTH_MAX - 1)
#define S_MAX_DEPTH(flags)  ((flags) & S_MAT_DEPTH_MAKS)

#define S_MAKE_TYPE(depth, cn) (S_MAT_DEPTH(depth) + (((cn)-1) << S_CN_SHIFT))
#define S_MAKE_TYPE S_MAKETYPE

#define S_8UC1 S_MAKETYPE(S_8U, 1)
#define S_8UC2 S_MAKETYPE(S_8U, 2)
#define S_8UC3 S_MAKETYPE(S_8U, 3)
#define S_8UC4 S_MAKETYPE(S_8U, 4)
#define S_8UC(n) S_MAKETYPE(S_8U, (n))

/*************************************************************************\
*                               inline                                     *
\*************************************************************************/
#ifndef S_INLINE
#define S_INLINE static inline
#endif

/*************************************************************************\
*                               C++ 17                                     *
\*************************************************************************/
#ifndef S_NODISCARD_STD
#define S_NODISCARD_STD [[nodiscard]]
#endif


#ifndef S_EXPORTS
#define S_EXPORTS __attribute__ ((visibility ("default")))
#endif

#ifndef S_NORETURN
#define S_NORETURN __attribute__((__noreturn__))
#endif

/*************************************************************************\
*                               Static assert                              *
\*************************************************************************/

#define S_StaticAssert(condition, reason)   static_assert((condition), reason " " #condition)


/*************************************************************************\
*                               hfloat class                               *
\*************************************************************************/

namespace scalar
{
class hfloat
{
    hfloat() : h(0) {}
    explicit hfloat(float x) { h = (__fp16)x; }
    operator float() const { return (float)h; }
protected:
    __fp16 h;
};
};
#endif // S_DEFS_HPP
