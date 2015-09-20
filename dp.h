/**
 * Copyright (c) 2015 Voidware Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 * 
 * contact@voidware.com
 */

#ifndef __dp_h__
#define __dp_h__

template<class D> struct DP
{
    /* Precison doubler.
     * given a class D supporting basic arithmetic (*+/-, and comparisions)
     * make a new class DP that doubles the precision.
     * D must be of floating point type. to start with D can be float
     * or double. 
     * DP can be applied to itself to re-double precision.
     */

    // Constraints on D
    // D::D(0);

     // Constructors
                        DP() {}
                        DP(const D& a) { f_[0] = 0; f_[1] = a; }

    // Features
    bool                operator==(const DP& a) const
                               { return f_[0] == a.f_[0] && f_[1] == a.f_[1]; }
    bool                operator!=(const DP& a) const
                               { return f_[0] != a.f_[0] || f_[1] != a.f_[1]; }

    bool                operator<(const DP&) const;
    bool                operator<=(const DP&) const;
    bool                operator>(const DP& b) const
                                { return b < *this; }
    bool                operator>=(const DP& b) const
                                { return b <= *this; }

    void                operator+=(const DP&);
    void                operator-=(const DP&);
    void                operator*=(const DP&);
    void                operator/=(const DP&);

    static DP<D>        makeSplit(const D& two)
                        {
                            return split_*split_ - split_ - split_ + two;
                        }

    D                   f_[2];
    static D            split_;
};


template<class D> D DP<D>::split_;

template<class D>
bool DP<D>::operator<(const DP<D>& b) const
{
    return f_[1] < b.f_[1] || f_[1] == b.f_[1] && f_[0] < b.f_[0];
}

template<class D>
bool DP<D>::operator<=(const DP<D>& b) const
{
    return f_[1] < b.f_[1] || f_[1] == b.f_[1] && f_[0] <= b.f_[0];
}

/* Computes fl(a+b) and err(a+b).  Assumes |a| >= |b|. */
#define QUICK_TWO_SUM(a, b, s, err)     \
    s = a + b;                          \
    err = b - (s - a)

/* Computes fl(a-b) and err(a-b).  Assumes |a| >= |b| */
#define QUICK_TWO_DIFF(a, b, s, err)    \
    s = a - b;                          \
    err = (a - s) - b

/* Computes fl(a+b) and err(a+b).  */
#define TWO_SUM(a, b, s, err)                           \
{                                                       \
    D bb;                                               \
    s = a + b;                                          \
    bb = s - a;                                         \
    err = (a - (s - bb)) + (b - bb);                    \
}

/* Computes fl(a-b) and err(a-b).  */
#define TWO_DIFF(a, b, s, err)                          \
{                                                       \
    D bb;                                               \
    s = a - b;                                          \
    bb = s - a;                                         \
    err = (a - (s - bb)) - (b + bb);                    \
}

/* Computes high word and lo word of a */
#define SPLIT_(a, hi, lo)                                \
{                                                       \
    D t = _split * a;                                    \
    hi = t - (t - a);                                   \
    lo = a - hi;                                        \
}

/* Computes fl(a*b) and err(a*b). */
#define TWO_PROD(a, b, p, err)                                  \
{                                                               \
    D a_hi, a_lo, b_hi, b_lo;                                   \
    p = a * b;                                                  \
    SPLIT_(a, a_hi, a_lo);                                       \
    SPLIT_(b, b_hi, b_lo);                                       \
    err = a_hi*b_hi - p + a_hi*b_lo + a_lo*b_hi + a_lo*b_lo;    \
}


/* This one satisfies IEEE style error bound, 
   due to K. Briggs and W. Kahan.
   */
#define TWO_TWO_SUM(a0, a1, b0, b1, x0, x1)                     \
{                                                               \
    D _s1, _s2, _t1, _t2, _u1, _u2;                             \
    TWO_SUM(a1, b1, _s1, _s2);                                  \
    TWO_SUM(a0, b0, _t1, _t2);                                  \
    _s2 += _t1;                                                 \
    QUICK_TWO_SUM(_s1, _s2, _u1, _u2);                          \
    _s2 = _u2 + _t2;                                            \
    QUICK_TWO_SUM(_u1, _s2, x1, x0);                            \
}

#define TWO_TWO_DIFF(a0, a1, b0, b1, x0, x1)                    \
{                                                               \
    D _s1, _s2, _t1, _t2, _u1, _u2;                             \
    TWO_DIFF(a1, b1, _s1, _s2);                                 \
    TWO_DIFF(a0, b0, _t1, _t2);                                 \
    _s2 += _t1;                                                 \
    QUICK_TWO_SUM(_s1, _s2, _u1, _u2);                          \
    _s1 = _u1;                                                  \
    _s2 = _u2 + _t2;                                            \
    QUICK_TWO_SUM(_s1, _s2, x1, x0);                            \
}

#define TWO_TWO_PROD(a0, a1, b0, b1, x0, x1)            \
{                                                       \
    D _p1, _p2;                                         \
    TWO_PROD(a1, b1, _p1, _p2);                         \
    _p2 += a1*b0;                                       \
    _p2 += a0*b1;                                       \
    QUICK_TWO_SUM(_p1, _p2, x1, x0);                    \
}

#define TWO_ONE_SUM(a0, a1, b, x0, x1)                  \
{                                                       \
    D _s1, _s2;                                         \
    TWO_SUM(a1, b, _s1, _s2);                           \
    _s2 += a0;                                          \
    QUICK_TWO_SUM(_s1, _s2, x1, x0);                    \
}

#define TWO_ONE_PROD(a0, a1, b, x0, x1)                 \
{                                                       \
    D _p1, _p2;                                         \
    TWO_PROD(a1, b, _p1, _p2);                          \
    _p2 += a0*b;                                        \
    QUICK_TWO_SUM(_p1, _p2, x1, x0);                    \
}

#define TWO_TWO_DIV(a0, a1, b0, b1, x0, x1)             \
{                                                       \
    D q1, q2;                                           \
    D _r0, _r1;                                         \
    D v0, v1, v2, v3;                                   \
    D q3;                                               \
    q1 = a1/b1;                                         \
    TWO_ONE_PROD(b0, b1, q1, v2, v3);                   \
    TWO_TWO_DIFF(a0, a1, v2, v3, v0, v1);               \
    q2 = v1/b1;                                         \
    TWO_ONE_PROD(b0, b1, q2, v2, v3);                   \
    TWO_TWO_DIFF(v0, v1, v2, v3, _r0, _r1);             \
    q3 = _r1/b1;                                        \
    QUICK_TWO_SUM(q1, q2, v0, v1);                      \
    TWO_ONE_SUM(q2, q1, q3, x0, x1);                    \
}

/* double-double / double */
#define TWO_ONE_DIV(a0, a1, b, x0, x1)                  \
{                                                       \
    D q1, q2;                                           \
    D _p1, _p2;                                         \
    D s, e;                                             \
    q1 = a1/b;                                          \
    TWO_PROD(q1, b, _p1, _p2);                          \
    TWO_DIFF(a1, _p1, s, e);                            \
    e = e + a0 - _p2;                                   \
    q2 = DIVOP(ADDOP(s, e), b);                         \
    QUICK_TWO_SUM(q1, q2, x1, x0);                      \
}

#define TWO_ONE_DIFF(a0, a1, b, x0, x1)                 \
{                                                       \
    D _s1, _s2;                                         \
    TWO_DIFF(a1, b, _s1, _s2);                          \
    _s2 += a0;                                          \
    QUICK_TWO_SUM(_s1, _s2, x1, x0);                    \
}

#define TWO_SQR(a, x0, x1)                              \
{                                                       \
    D hi, lo, t;                                        \
    x1 = a*a;                                           \
    SPLIT_(a, hi, lo);                                   \
    t = hi*lo;                                          \
    x0 = hi*hi - x1 + t + t + lo*lo;                    \
}

#define TWO_TWO_SQR(a0, a1, x0, x1)                     \
{                                                       \
    D _p1, _p2;                                         \
    D _s1, _s2;                                         \
    D t;                                                \
    TWO_SQR(a1, _p2, _p1);                              \
    t = a1*a0;                                          \
    _p2 += t + t - a0*a0;                               \
    QUICK_TWO_SUM(_p1, _p2, x1, x0);                    \
}


template<class D>
void DP<D>::operator+=(const DP<D>& a)
{
    TWO_TWO_SUM(a.f_[0], a.f_[1], f_[0], f_[1], f_[0], f_[1]);        
}

template<class D>
void DP<D>::operator-=(const DP<D>& a)
{
    TWO_TWO_DIFF(f_[0], f_[1], a.f_[0], a.f_[1], f_[0], f_[1]);        
}

template<class D>
void DP<D>::operator*=(const DP<D>& a)
{
    const D& _split = DP<D>::split_;
    TWO_TWO_PROD(a.f_[0], a.f_[1], f_[0], f_[1], f_[0], f_[1]);        
}

template<class D>
void DP<D>::operator/=(const DP<D>& a)
{
    const D& _split = DP<D>::split_;
    TWO_TWO_DIV(f_[0], f_[1], a.f_[0], a.f_[1], f_[0], f_[1]);        
}

template<class D> DP<D> operator+(const DP<D>& a, const DP<D>& b)
{
    DP<D> c;
    TWO_TWO_SUM(a.f_[0], a.f_[1], b.f_[0], b.f_[1], c.f_[0], c.f_[1]);    
    return c;
}

template<class D> DP<D> operator-(const DP<D>& a, const DP<D>& b)
{
    DP<D> c;
    TWO_TWO_DIFF(a.f_[0], a.f_[1], b.f_[0], b.f_[1], c.f_[0], c.f_[1]);    
    return c;
}

template<class D> DP<D> operator*(const DP<D>& a, const DP<D>& b)
{
    DP<D> c;
    const D& _split = DP<D>::split_;
    TWO_TWO_PROD(a.f_[0], a.f_[1], b.f_[0], b.f_[1], c.f_[0], c.f_[1]);    
    return c;
}

template<class D> DP<D> operator/(const DP<D>& a, const DP<D>& b)
{
    DP<D> c;
    const D& _split = DP<D>::split_;
    TWO_TWO_DIV(a.f_[0], a.f_[1], b.f_[0], b.f_[1], c.f_[0], c.f_[1]);    
    return c;
}

#endif /* __dp_h__ */


