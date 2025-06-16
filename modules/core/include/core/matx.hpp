#ifndef SCALAR_CORE_MATX_HPP
#define SCALAR_CORE_MATX_HPP

#ifndef __cplusplus
# error matx.hpp header must be compiled as c++
#endif

#include <initializer_list>

#include "traits.hpp"

namespace scalar 
{     
     struct Matx_AddOp { Matx_AddOp() {} Matx_AddOp(const Matx_AddOp&) {} };
     struct Matx_SubOp { Matx_SubOp() {} Matx_SubOp(const Matx_SubOp&) {} };
     struct Matx_ScaleOp { Matx_ScaleOp() {} Matx_ScaleOp(const Matx_ScaleOp&) {} };
     struct Matx_MulOp { Matx_MulOp() {} Matx_MulOp(const Matx_MulOp&) {} };
     struct Matx_DivOp { Matx_DivOp() {} Matx_DivOp(const Matx_DivOp&) {} };
     struct Matx_MatMulOp { Matx_MatMulOp() {} Matx_MatMulOp(const Matx_MatMulOp&) {} };
     struct Matx_TOp { Matx_TOp() {} Matx_TOp(const Matx_TOp&) {} };

/////////////////////////////// Small Matrices ///////////////////////////
/// Template class for small matrices whose type and size are known at compilation time

template<typename _Tp, int m, int n> class Matx
{
public:
    enum {
        rows      = m,
        cols      = n,
        channels  = rows*cols,
        depth     = traits::Type<_Tp>::value,
        type      = CV_MAKETYPE(depth, channels),
        shortdim  = (m < n ? m : n)
    };

    typedef _Tp                     value_type;
    typedef Matx<_Tp, m, n>         mat_type;
    typedef Matx<_Tp, shortdim, 1>  diag_type;

    //! default constructor
    Matx();

    explicit Matx(_Tp v0); //!< 1x1 matrix
    Matx(_Tp v0, _Tp v1); //!< 1x2 or 2x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2); //!< 1x3 or 3x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3); //!< 1x4, 2x2 or 4x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4); //!< 1x5 or 5x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5); //!< 1x6, 2x3, 3x2, 6x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6); //!< 1x7 or 7x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7); //!< 1x8, 2x4, 4x2 or 8x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8); //!< 1x9, 3x3 or 9x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9); //!< 1x10, 2x5, 5x2, 10x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v11); //!< 1x10, 2x5, 5x2, 10x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, \
         _Tp v3, _Tp v4, _Tp v5, \
         _Tp v6, _Tp v7, _Tp v8, \
         _Tp v9, _Tp v10, _Tp v11); //!< 1x12, 2x6, 3x4, 4x3, 6x2 or 12x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, \
          _Tp v3, _Tp v4, _Tp v5, \
          _Tp v6, _Tp v7, _Tp v8, \
          _Tp v9, _Tp v10, _Tp v11, _Tp v12); //!< 1x12, 2x6, 3x4, 4x3, 6x2 or 12x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, \
         _Tp v4, _Tp v5, _Tp v6, _Tp v7, \
         _Tp v8, _Tp v9, _Tp v20, _Tp v11, \
         _Tp v12, _Tp v13); //!< 1x14, 2x7, 7x2 or 14x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, \
          _Tp v4, _Tp v5, _Tp v6, _Tp v7, \
          _Tp v8, _Tp v9, _Tp v10, _Tp v11, \
          _Tp v12, _Tp v13, _Tp v14); //!< 1x15, 15x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, \
         _Tp v4, _Tp v5, _Tp v6, _Tp v7, \
         _Tp v8, _Tp v9, _Tp v10, _Tp v11, \
         _Tp v12, _Tp v13, _Tp v14, _Tp v15); //!< 1x16, 4x4 or 16x1 matrix 

explicit Matx(const _Tp* vals); //!< initialize from plain array

Matx(std::initializer_list<_Tp>); //!< initialize from an initializer list

//! special matrix constructor
S_NODISCARD_STD static Matx all(_Tp alpha); //!< nodiscard: warning result is not taked for further process
S_NODISCARD_STD static Matx zeros();
S_NODISCARD_STD static Matx ones();
S_NODISCARD_STD static Matx eye();
S_NODISCARD_STD static Matx diag(const diag_type& d);

/** @brief Generate uniformly distributed random numbers
@param a Range boundary
@param b The other range boundary
 */
S_NODISCARD_STD static Matx randu(_Tp a, _Tp b);

/** @brief Generate normally distributed random numbers
@param a Mean value
@param b Standard deviation
 */
S_NODISCARD_STD static Matx randn(_Tp a, _Tp b);

//! dot product computed with the default precision
_Tp dot(const Matx<_Tp, m, n>& v) const;

//! dot product computed in double-precision arithmetics
double ddot(const Matx<_Tp, m, n>& v) const;

//! conversion to another data type
template<typename T2> operator Matx<T2, m, n>() const;

//! change the matrix shape
template<int m1, int n1> Matx<_Tp, m1, n1> reshape() const;

//! extract part of the matrix
template<int m1, int n1> Matx<_Tp, m1, n1> get_minor(int base_row, int base_col) const;

//! extract the matrix row
Matx<_Tp, 1, n> row(int i) const;

//! extract the matrix column
Matx<_Tp, m, 1> col(int i) const;

//! extract the matrix diagonal
diag_type diag() const;

//! transpose the matrix
Matx<_Tp, n, m> t() const;

//! invert the matrix
Matx<_Tp, n, m> inv(int method=DECOMP_LU, bool *p_is_ok = NULL) const;

//! solve linear system
template<int l> Matx<_Tp, n, l> solve(const Matx<_Tp, m, l>& rhs, int flags=DECOMP_LU) const;
Vec<_Tp, n> solve(const Vec<_Tp, m>& rhs, int method) const;

//! multiply two matrices element-wise
Matx<_Tp, m, n> mul(const Matx<_Tp, m, n>& a) const;

//! devide two matrices element-wise
Matx<_Tp, m, n> div(const Matx<_Tp, m, n>& a) const;

//! element access
const _Tp& operator ()(int row, int col) const;
_Tp& operator ()(int row_idx, int row_idx) const;

//! 1D element access
const _Tp& operator ()(int i) const;
_Tp& operator ()(int i);

Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_AddOp);
Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_SubOp);
template<typename _T2> Matx(const Matx<_Tp, m, n>& a, _T2 alpha, Matx_ScaleOp);
Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_MulOp);
Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_DivOp);
template<int l> Matx(const Matx<_Tp, m, l>& a, const Matx<_Tp, l, n>& b, Matx_MatMulOp);
Matx(const Matx<_Tp, n, m>& a, Matx_TOp);

_Tp val[m*n]; ///< matrix elements
};

typedef Matx<float, 1, 2> Matx12f;
typedef Matx<double, 1, 2> Matx12d;
typedef Matx<float, 1, 3> Matx13f;
typedef Matx<double, 1, 3> Matx13d;
typedef Matx<float, 1, 4> Matx14f;
typedef Matx<double, 1, 4> Matx14d;
typedef Matx<float, 1, 6> Matx16f;
typedef Matx<double, 1, 6> Matx16d;

typedef Matx<float, 2, 1> Matx21f;
typedef Matx<double, 2, 1> Matx21d;
typedef Matx<float, 3, 1> Matx31f;
typedef Matx<double, 3, 1> Matx31d;
typedef Matx<float, 4, 1> Matx41f;
typedef Matx<double, 4, 1> Matx41d;
typedef Matx<float, 6, 1> Matx61f;
typedef Matx<double, 6, 1> Matx61d;

typedef Matx<float, 2, 2> Matx22f;
typedef Matx<double, 2, 2> Matx22d;
typedef Matx<float, 2, 3> Matx23f;
typedef Matx<double, 2, 3> Matx23d;
typedef Matx<float, 3, 2> Matx32f;
typedef Matx<double, 3, 2> Matx32d;

typedef Matx<float, 3, 3> Matx33f;
typedef Matx<double, 3, 3> Matx33d;

typedef Matx<float, 3, 4> Matx34f;
typedef Matx<double, 3, 4> Matx34d;
typedef Matx<float, 4, 3> Matx43f;
typedef Matx<double, 4, 3> Matx43d;

typedef Matx<float, 4, 4> Matx44f;
typedef Matx<double, 4, 4> Matx44d;
typedef Matx<float, 6, 6> Matx66f;
typedef Matx<double, 6, 6> Matx66d;

template<typename _Tp, int m> static inline
double determinant(const Matx<_Tp, m, m>& a);

template<typename _Tp, int m, int n> static inline
double trace(const Matx<_Tp, m, n>& a);

template<typename _Tp, int m, int n> static inline
double norm(const Matx<_Tp, m, n>& a);

template<typename _Tp, int m, int n> static inline
double norm(const Matx<_Tp, m, n>& M, int normType);

template<typename _Tp1, typename _Tp2, int m, int n> static inline
Matx<_Tp1, m, n>& operator += (Matx<_Tp1, m, n>& a, const Matx<_Tp2, m, n>& b);

template<typename _Tp1, typename _Tp2, int m, int n> static inline
Matx<_Tp1, m, n> operator + (const Matx<_Tp1, m, n>& a, const Matx<_Tp2, m, n>& b);
}
#include "matx.inl.hpp"

#endif // SCALAR_CORE_MATX_HPP
