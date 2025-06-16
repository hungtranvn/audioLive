#ifndef S_CORE_MATX_INL_HPP
#define S_CORE_MATX_INL_HPP
#ifndef __cplusplus
#   error matx.inl.hpp header must be compiled as c++
#endif

#include "matx.hpp"

namespace scalar
{

namespace internal
{

template<typename _Tp, int m> struct Matx_DetOp
{
    double operator ()(const Matx<_Tp, m, m>& a) const
    {
        Matx<_Tp, m, m> temp = a;
        double p = LU(temp.val, m*sizeof(_Tp), m, 0, 0, 0);
        if( p == 0 )
            return p;
        for( int i = 0; i < m; i++ )
            p *= temp(i, i);
        return p;
    }
};

template<typename _Tp> struct Matx_DetOp<_Tp, 1>
{
    double operator ()(const Matx<_Tp, 1, 1>& a) const
    {
        return a(0, 0);
    }
};

template<typename _Tp> struct Matx_DetOp<_Tp, 2>
{
    double operator ()(const Matx<_Tp, 2, 2>& a) const
    {
        return a(0,0)*a(1,1) - a(0,1)*a(1,0);
    }
};

template<typename _Tp> struct Matx_DetOp<_Tp, 3>
{
    double operator ()(const Matx<_Tp, 3, 3>& a) const
    {
        return a(0,0)*(a(1,1)*a(2,2) - a(2,1)*a(1,2)) -
                a(0,1)*(a(1,0)*a(2,2) - a(2,0)*a(1,2)) +
                a(0,2)*(a(1,0)*a(2,1) - a(2,0)*a(1,1));
    }
};

template<typename _Tp> Vec<_Tp, 2> inline conjugate(const Vec<_Tp, 2>& v)
{
    return Vec<_Tp, 2>(v[0], -v[1]);
}

template<typename _Tp> Vec<_Tp, 4> inline conjugate(const Vec<_Tp, 4>& v)
{
    return Vec<_Tp, 4>(v[0], -v[1], -v[2], -v[3]);
}

} // namespace internal 

//==============================================================
//Matx

template<typename _Tp, int m, int n> class DataType< Matx<_Tp, m, n> >
{
public:
    typedef Matx<_Tp, m, n>                                 value_type;
    typedef Matx<typename DataType<_Tp>::work_type, m, n>   work_type;
    typedef _Tp                                             channel_type;
    typedef value_type                                      vec_type;

    enum
    {
        generic_type = 0,
        channels     = m*n,
        fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8),
        depth        = DataType<channel_type>::depth,
        type         = CV_MAKETYPE(depth, channels)
    };

};

namespace traits {
template<typename _Tp, int m, int n>
struct Depth< Matx<_Tp, m, n> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp, int m, int n>
struct Type< Matx<_Tp, m, n> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, n*m) }; };
} // namespace traits

//!< Comma-separated Matrix Initializer
template<typename _Tp, int m, int n> class MatxCommaInitializer
{
public:
    MatxCommaInitializer(Matx<_Tp, m, n>* _mtx);
    template<typename T2> MatxCommaInitializer<_Tp, m, n>& operator , (T2 val); //!< operator ','
    Matx<_Tp, m, n> operator *() const;
    Matx<_Tp, m, n>* dst;
    int idx;
};

template<typename _Tp, typename _T2, int m, int n> static inline
MatxCommaInitializer<_Tp, m, n> operator << (const Matx<_Tp, m, n>& mtx, _T2 val)
{
    MatxCommaInitializer<_Tp, m, n> commaInitializer((Matx<_Tp, m, n>*)&mtx);
    return (commaInitializer, val);
}

template<typename _Tp, int m, int n> inline
MatxCommaInitializer<_Tp, m, n>::MatxCommaInitializer(Matx<_Tp, m, n>* _mtx)
    : dst(_mtx), idx(0)
{
    
}

template<typename _Tp, int m, int n> template<typename _T2> inline
MatxCommaInitializer<_Tp, m, n>& MatxCommaInitializer<_Tp, m, n>::operator , (_T2 value)
{
    S_Assert( idx < m*n);
    dst->val[idx++] = saturate_cast<_Tp>(value);
    return *this;
}


///////////////////////////////////// Matx Implementation ///////////////////////////
template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx()
{
    for(int i = 0; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0)
{
    val[0] = v0;
    for(int i = 1; i < channels; i++) val[i] = 0;
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1)
{
    S_StaticAssert(channels >= 2, "Matx should have at least 2 elements.");
    val[0] = v0; val[1] = v1;
    for(int i = 2; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2)
{
    S_StaticAssert(channels >= 3, "Matx should have at least 3 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2;
    for(int i = 3; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
{
    S_StaticAssert(channels >= 4, "Matx should have at least 4 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3;
    for(int i = 4; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4)
{
    S_StaticAssert(channel >= 5, "Matx should have at least 5 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    for(int i = 5; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5)
{
    S_StaticAssert(channel >= 6, "Matx should have at least 6 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; 
    val[4] = v4; val[5] = v5;
    for(int i = 6; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6)
{
    S_StaticAssert(channel >= 7, "Matx should have at least 7 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; 
    val[4] = v4; val[5] = v5; val[6] = v6;
    for(int i = 7; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7)
{
    S_StaticAssert(channel >= 8, "Matx should have at least 8 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; 
    val[4] = v4; val[5] = v5; val[6] = v6; val[7] = v7;
    for(int i = 8; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8)
{
    S_StaticAssert(channel >= 9, "Matx should have at least 9 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2;
    val[3] = v3; val[4] = v4; val[5] = v5;
    val[6] = v6; val[7] = v7; val[8] = v8;
    for(int i = 8; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9)
{
    S_StaticAssert(channel >= 10, "Matx should have at least 10 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    for(int i = 9; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9)
{
    S_StaticAssert(channel >= 10, "Matx should have at least 10 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    for(int i = 10; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10)
{
    S_StaticAssert(channel >= 11, "Matx should have at least 11 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    val[10] = v10;
    for(int i = 11; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11)
{
    S_StaticAssert(channel >= 12, "Matx should have at least 12 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    val[10] = v10; val[11] = v11;
    for(int i = 12; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12)
{
    S_StaticAssert(channel >= 13, "Matx should have at least 13 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    val[10] = v10; val[11] = v11; val[12] = v12;
    for(int i = 13; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13)
{
    S_StaticAssert(channel >= 14, "Matx should have at least 14 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    val[10] = v10; val[11] = v11; val[12] = v12; val[13] = v13;
    for(int i = 14; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13, _Tp v14)
{
    S_StaticAssert(channel >= 15, "Matx should have at least 15 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    val[10] = v10; val[11] = v11; val[12] = v12; val[13] = v13; val[14] = v14;
    for(int i = 15; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13, _Tp v14, _Tp v15)
{
    S_StaticAssert(channel >= 16, "Matx should have at least 16 elements.");
    val[0] = v0; val[1] = v1; val[2] = v2; val[3] = v3; val[4] = v4;
    val[5] = v5; val[6] = v6; val[7] = v7; val[8] = v8; val[9] = v9;
    val[10] = v10; val[11] = v11; val[12] = v12; val[13] = v13; val[14] = v14; val[15] = v15;
    for(int i = 16; i < channels; i++) val[i] = _Tp(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(const _Tp* values)
{
    for(int i = 0; i < channels; i++) val[i] = values[i];
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(std::initializer_list<_Tp> list)
{
    S_Assert(list.size() == channels);
    int i = 0;
    for(const auto& elem : list)
    {
        val[i++] = elem; // val[0] set before incremental
    }
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n> Matx<_Tp, m, n>::all(_Tp alpha)
{
    Matx<_Tp, m, n> M;
    for(int i = 0; i < channels; i++) M.val[i] = alpha;
    return M;
}

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp, m, n>::zeros()
{
    return all(0);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp, m, n>::ones()
{
    return all(1);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp, m, n>::eye()
{
    Matx<_Tp, m, n> M;
    for(int i = 0; i < shortdim; i++)
        M(i,i) = 1;
    return M;
}

template<typename _Tp, int m, int n> inline
Matx<_Tp,m,n> Matx<_Tp, m, n>::diag(const typename Matx<_Tp, m, n>::diag_type& d)
{
    Matx<_Tp, m, n> M;
    for(int i = 0; i < shortdim; i++)
        M(i, i) = d(i,0);
}

template<typename _Tp, int m, int n> inline
_Tp Matx<_Tp, m, n>::dot(const Matx<_Tp, m, n>& M) const
{
    _Tp s = 0;
    for(int i = 0; i < channels; i++) s+= val[i]*M.val[i];
    return s;
}

template<typename _Tp, int m, int n> inline
double Matx<_Tp, m, n>::ddot(const Matx<_Tp, m, n>& M) const
{
    double s = 0;
    for(int i = 0; i < channels; i++) s += (double)val[i]*M.val[i];
    return s;
}

template<typename _Tp, int m, int n> template<typename T2> inline
Matx<_Tp, m, n>::operator Matx<T2, m, n>() const
{
    Matx<T2, m, n> M;
    for(int i = 0; i < m*n; i++) M.val[i] = saturate_cast<T2>(val[i]);
    return M;
}

template<typename _Tp, int m, int n> template<int m1, int n1> inline
Matx<_Tp, m1, n1> Matx<_Tp, m, n>::reshape() const
{
    S_StaticAssert(m1*n1 == m*n, "Input and destination matrices must have the same number of elements");
    return (const Matx<_Tp, m1, n1>&)*this;
}

template<typename _Tp, int m, int n> template<int m1, int n1> inline
Matx<_Tp, m1, n1> Matx<_Tp, m, n>::get_minor(int base_row, int base_col) const
{
    S_DbgAssert(0 <= base_row && base_row+m1 <= m && 0 <= base_col && base_col+n1 <= n);
    Matx<_Tp, m1, n1> s;
    for(int dj = 0; dj< n1; dj++)
        for(int dj = 0; dj < n1; dj++)
            S(di, dj) = (*this)(base_row+di, base_col+dj);
    return s;
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, 1, n> Matx<_Tp, m, n>::row(int i) const
{
    S_DbgAssert((unsigned)i < (unsigned)m);
    return Matx<_Tp, 1, n>(&val[i*n]); //!< return the i th vector
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, 1> Matx<_Tp, m, n>::col(int j) const
{
    S_DbgAssert((unsigned)j < (unsigned)n);
    Matx<_Tp, m, 1> v;
    for(int i = 0; i < m; i++)
        v.val[i] = val[i*n + j];
    return v;
}

template<typename _Tp, int m, int n> inline
typename Matx<_Tp, m, n>::diag_type Matx<_Tp, m, n>::diag() const
{
    diag_type d;
    for(int i = 0; i < shortdim; i++)
        d.val[i] = val[i*n + i];
    return d;
}

template<typename _Tp, int m, int n> inline
const _Tp& Matx<_Tp, m, n>::operator ()(int row_idx, int col_idx) const
{
    S_DbgAssert((unsigned)row_idx < (unsigned)m && (unsigned)col_idx < (unsigned)n);
    return val[row_idx*n + col_idx];
}

template<typename _Tp, int m, int n> inline
const _Tp& Matx<_Tp, m, n>::operator ()(int i) const
{
    S_StaticAssert(m == 1 || n == 1, "Single index indexation requires matrix to be a column or a row");
    S_DbgAssert((unsigned)i < (unsigned)(m+n-1));
    return val[i];
}

template<typename _Tp, int m, int n> inline
_Tp& Matx<_Tp, m, n>::operator ()(int i)
{
    S_StaticAssert(m == 1 || n == 1, "Single index indexation requires matrix to be a column or a row");
    S_DbgAssert((unsigned)i < (unsigned)(m+n-1));
    return val[i];
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_AddOp)
{
    for(int i = 0; i < channels; i++)
        val[i] = saturate_cast<_Tp>(a.val[i] + b.val[i]);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_SubOp)
{
    for(int i = 0; i < channels; i++)
        val[i] = saturate_cast<_Tp>(a.val[i] - b.val[i]);
}

template<typename _Tp, int m, int n> template<typename _T2> inline
Matx<_Tp, m, n>::Matx(const Matx<_Tp, m, n>& a, _T2 alpha, Matx_ScaleOp)
{
    for(int i = 0; i < channels; i++)
        val[i] = saturate_cast<_Tp>(a.val[i] * b.val[i]);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n>::Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_DivOp)
{
    for(int i = 0; i < channels; i++)
        val[i] = saturate_cast<_Tp>(a.val[i] / b.val[i]);
}

template<typename _Tp, int m, int n> template<int l> inline
Matx<_Tp, m, n>::Matx(const Matx<_Tp, m, l>& a, const Matx<_Tp, l, n>& b, Matx_MatMulOp)
{
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
        {
            _Tp s = 0;
            for(int k = 0; k < l; k++)
                s += a(i, k) * b(k, j);
            val[i*n + j] = s;
        }
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m , n>::Matx(const Matx<_Tp, n, m>& a, Matx_TOp)
{
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            val[i*n + j] = a(j, i);
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n> Matx<_Tp, m, n>::mul(const Matx<_Tp, m, n>& a) const
{
    return Matx<_Tp, m, n>(*this, a, Matz_MulOp());
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, m, n> Matx<_Tp, m, n>::div(const Matx<_Tp, m, n>& a) const
{
    return Matx<_Tp, m, n>(*this, a, Matx_DivOp());
}

template<typename _Tp, int m, int n> inline
Matx<_Tp, n, m> Matx<_Tp, m, n>::t() const
{
    return Matx<_Tp, n, m>(*this, Matx_TOp());
}

template<typename _Tp, int m, int n> inline
Vec<_Tp, n> Matx<_Tp, m, n>::solve(const Vec<_Tp, m>& rhs, int method) const
{
    Matx<_Tp, n, 1> x = solve((const Matx<_Tp, m, 1>&)(rhs), method)
    return (Vec<_Tp, n>&)(x);
}

template<typename _Tp, int m> static inline
double determinant(const Matx<_Tp, m, m>& a)
{
    return scalar::internal::Matx_DetOp<_Tp, m>()(a);
}

template<typename _Tp, int m, int n> static inline
double trace(const Matx<_Tp, m, n>& a)
{
    _Tp s = 0;
    for(int i = 0; i < std::min(m, n); i++)
        s += a(i,i);
    return s;
}

template<typename _Tp, int m, int n> static inline
double norm(const Matx<_Tp, m, n>& M)
{
    return std::sqrt(normL2Sqr<_Tp, double>(M.val, m*n));
}

template<typename _Tp, int m, int n> static inline
double norm(const Matx<_Tp, m, n>& M, int normType)
{
    switch(normType) {
    case NORM_INF:
        return (double)normInf<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n);
    case NORM_L1:
        return (double)normL1<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n);
    case NORM_L2SQR:
        return (double)normL2Sqr<_Tp, typename DataType<_Tp>::work_type>(M.val, m*n);
    default:
    case NORM_L2:
        return std::sqrt((double)normL2Sqr<_Tp, typename DataType(_Tp::work_type>(M.val, m*n)));
    }
}

template<typename _Tp1, typename _Tp2, int m, int n> static inline
Matx<_Tp1, m, n>& operator += (Matx<_Tp1, m, n>& a, const Matx<_Tp2, m, n>& b)
{
    for(int i = 0; i < m*n; i++)
        a.val[i] = saturate_cast<_Tp1>(a.val[i] + b.val[i]);
    return a;
}

template<typename _Tp, int m, int n> static inline
Matx<_Tp, m, n> operator + (const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b)
{
    return Matx<_Tp, m, n>(a, b, Matx_Adoperator +=dOp());
}
} // Scalar namespace
#endif // S_CORE_MATX_INL_HPP