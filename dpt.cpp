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

#include <stdio.h>
#include "dp.h"

/* Test Program for precision doubling. */

int main()
{
    /* D2 will be a double double */
    typedef DP<double> D2;
    D2::split_ = double((1<<27)+1);

    D2 a, b;

    a += b;
    a -= b;
    a *= b;
    a /= b;

    /* D3 is a double D2 (ie quad precision) */
    typedef DP<D2> D3;
    D3::split_ = D2::makeSplit(2.0);

    D3 x, y;
    x += y;
    x -= y;
    x *= y;
    x /= y;

    /* now D4 is a double D3 (ie oct precision) */
    typedef DP<D3> D4;
    D4::split_ = D3::makeSplit(D2(2.0));
    {
        D4 x, y;
        x += y;
        x -= y;
        x *= y;
        x /= y;
    }
    
    return 0;
}
