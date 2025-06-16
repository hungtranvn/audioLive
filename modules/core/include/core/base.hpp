#ifndef SCALAR_CORE_BASE_HPP
#define SCALAR_CORE_BASE_HPP

#include "defs.hpp"
#include <climits>
#include <algorithm>

namespace scalar
{
namespace Error {

enum Code
{
    StsOk=                       0,  //!< everything is ok
    StsBackTrace=               -1,  //!< pseudo error for back trace
    StsError=                   -2,  //!< unknown /unspecified error
    StsInternal=                -3,  //!< internal error (bad state)
    StsNoMem=                   -4,  //!< insufficient memory
    StsBadArg=                  -5,  //!< function arg/param is bad
    StsBadFunc=                 -6,  //!< unsupported function
    StsNoConv=                  -7,  //!< iteration didn't converge
    StsAutoTrace=               -8,  //!< tracing
    HeaderIsNull=               -9,  //!< image header is NULL
    BadImageSize=              -10,  //!< image size is invalid
    BadOffset=                 -11,  //!< offset is invalid
    BadDataPtr=                -12,  //!<
    BadStep=                   -13,  //!< image step is wrong, this may happen for a non-continuous matrix.
    BadModelOrChSeq=           -14,  //!<
    BadNumChannels=            -15,  //!< bad number of channels, for example, some functions accept only single channel matrices.
    BadNumChannel1U=           -16,  //!<
    BadDepth=                  -17,  //!< input image depth is not supported by the function
    BadAlphaChannel=           -18,  //!<
    BadOrder=                  -19,  //!< number of dimensions is out of range
    BadOrigin=                 -20,  //!< incorrect input origin
    BadAlign=                  -21,  //!< incorrect input align
    BadCallBack=               -22,  //!<
    BadTileSize=               -23,  //!<
    BadCOI=                    -24,  //!< input COI is not supported
    BadROISize=                -25,  //!< incorrect input roi
    MaskIsTiled=               -26,  //!<
    StsNullPtr=                -27,  //!< null pointer
    StsVecLengthErr=           -28,  //!< incorrect vector length
    StsFilterStructContentErr= -29,  //!< incorrect filter structure content
    StsKernelStructContentErr= -30,  //!< incorrect transform kernel content
    StsFilterOffsetErr=        -31,  //!< incorrect filter offset value
    StsBadSize=                -201, //!< the input/output structure size is incorrect
    StsDivByZero=              -202, //!< division by zero
    StsInplaceNotSupported=    -203, //!< in-place operation is not supported
    StsObjectNotFound=         -204, //!< request can't be completed
    StsUnmatchedFormats=       -205, //!< formats of input/output arrays differ
    StsBadFlag=                -206, //!< flag is wrong or not supported
    StsBadPoint=               -207, //!< bad CvPoint
    StsBadMask=                -208, //!< bad format of mask (neither 8uC1 nor 8sC1)
    StsUnmatchedSizes=         -209, //!< sizes of input/output structures do not match
    StsUnsupportedFormat=      -210, //!< the data format/type is not supported by the function
    StsOutOfRange=             -211, //!< some of parameters are out of range
    StsParseError=             -212, //!< invalid syntax/structure of the parsed file
    StsNotImplemented=         -213, //!< the requested function/feature is not implemented
    StsBadMemBlock=            -214, //!< an allocated block has been corrupted
    StsAssert=                 -215, //!< assertion failed
    GpuNotSupported=           -216, //!< no CUDA support
    GpuApiCallError=           -217, //!< GPU API call error
    OpenGlNotSupported=        -218, //!< no OpenGL support
    OpenGlApiCallError=        -219, //!< OpenGL API call error
    OpenCLApiCallError=        -220, //!< OpenCL API call error
    OpenCLDoubleNotSupported=  -221,
    OpenCLInitError=           -222, //!< OpenCL initialization error
    OpenCLNoAMDBlasFft=        -223
};

}// namespace error

//! matrix decomposition types
enum DecompTypes
{
    DECOMP_LU       = 0, //!< Gaussian elimination with optimal pivot element chosen
    DECOMP_SVD      = 1, //!< Singular value decomposition method, the matrix can be singular
    DECOMP_EIG      = 2, //!< eigen value decomposition, the matrix must be symetrical
    DECOMP_CHOLESKY =3,  //!< Cholesky factorization, the matrix can be singular
    DECOMP_QR       = 4, //!< QR factorization, the matrix can be singular
    DECOMP_NORMAL   = 16 //!< Normal equation
};

//! norm type
enum NormTypes
{
    NORM_INF        = 1,
    NORM_L1         = 2,
    NORM_L2         = 4,
    NORM_L2SQR      = 5,
    NORM_HAMMING    = 6,   
    NORM_HAMMING2   = 7,
    NORM_TYPE_MASK  = 7, //!< bit-mask
    NORM_RELATIVE   = 8, //!< flag
    NORM_MINMAX     = 32 //!< flag

};

//! comparation types
enum CmpType
{
    CMP_EQ  = 0,
    CMP_GT  = 1,
    CMP_GE  = 2,
    CMP_LT  = 3,
    CMP_LE  = 4,
    CMP_NE  = 5
};

//! generalized matrix multiplication flahgs
enum GemmFlags
{
    GEMM_1_T    = 1,
    GEMM_2_T    = 2,
    GEMM_3_T    = 4
};

enum DftFlags
{
    DFT_INVERSE         = 1,
    DFT_SCALE           = 2,
    DFT_ROWS            = 4,
    DFT_COMPLEX_OUTPUT  = 16,
    DFT_REAL_OUTPUT     = 64,
    DCT_INVERSE         = DFT_INVERSE,
    DCT_ROWS            = DFT_ROWS
};

//! border types, image boundaries
enum BorderTypes
{
    BORDER_CONSTANT     = 0,
    BORDER_REPLICATE    = 1,
    BORDER_REFLECT      = 2,
    BORDER_WRAP         = 3,
    BORDER_REFLECT_101  = 4,
    BORDER_TRANSPARENT  = 5,
    BORDER_REFLECT101   = BORDER_REFLECT_101,
    BORDER_DEFAULT      = BORDER_REFLECT_101,
    BORDER_ISOLATED     = 16
};

/*! @brief Signals an error and raises the exception
By default the function prints information about the error to stderr,
then it either stops if setBreakOnError() had been called before or raises the exception.
It is possible to alternate error processing by using redirectError().
@param code - error code (Error::Code)
@param err - error description
@param func - function name. Available only when the compiler supports getting it
@param file - source file name where the error has occurred
@param line - line number in the source file where the error has occurred
@see S_Error, S_Error_, S_Assert, S_DbgAssert
*/
S_EXPORTS S_NORETURN void error(int code, const String& err, const char* func, const char* file, int line);

/*! @brief Signals an error and terminate application.

By default the function prints information about the error to stderr, then it terminates application
with std::terminate. The function is designed for invariants check in functions and methods with
noexcept attribute.
@param code - error code (Error::Code)
@param err - error description
@param func - function name. Available only when the compiler supports getting it
@param file - source file name where the error has occurred
@param line - line number in the source file where the error has occurred
@see S_AssertTerminate
 */
S_EXPORTS S_NORETURN void terminate(int code, const String& err, const char* func, const char* file, int line) CV_NOEXCEPT;

#ifdef S_STATIC_ANALYSIS

#define S_Error(code, msg) do { (void)(code); (void)(msg); abort();} while (0)
#define S_Error_(code, args) do { (void)(code); (void)(scalar::format args); abort(); } while (0)
#define S_Assert(expr) do { if (!(expr)) abort();} while (0)

#else
/** @brief Call the error handler
Currently, the error handler prints the error code and the error msg to standard error stream "stderr".
In the debug configuration, it then provokes memory access violation,
so that the execution stack and all the parameters can be analyzed by the debugger.
In the release configuration, the exception is thrown.
*/
#define S_Error(code, msg) scalar::error( code, msg, S_Func, __FILE__, __LINE__ )

/** @brief Call the error handler
 * This macro can be used to construct an error msg on-fly to include some dynamic information
 * 
 */
#define S_Error_(code, msg) scalar::error( code, scalar::format args, S_Func, __FILE, __LINE__)

#define S_Assert(expr) do { if(!!(expr)); else scalar::error(scalar::Error::StsAssert, #expr, S_Func, __FILE__, __LINE__); } while(0)
#define S_AssertTerminate(expr) do { if(!!(expr)); else scalar::terminate( #expr, S_Func, __FILE__, __LINE__); } while(0)
#define S_DbgAssert(expr) S_Assert(expr)
#endif

/*
* Hamming distance functor - counts the bit differences between two strings
*/

struct Hamming
{
    static const NormTypes normType = NORM_HAMMING;
    typedef unsigned char ValueType;
    typedef int ResultType;

    ResultType operator()(const unsigned char* a, const unsigned char* b, int size) const;
};

typedef Hamming HammingLUT;

///////////////////////////////////// inline norms ////////////////////////////////////////
template<typename _Tp> inline _Tp s_abs(_Tp x) {return std::abs(x);}
inline int s_abs(uchar x) {return x;}
inline int s_abs(schar x) {return std::abs(x);}
inline int s_abs(ushort x) {return x;}
inline int s_abs(short x) {return std::abs(x);}


} // namespace scalar

#endif // SCALAR_CORE_BASE_HPP