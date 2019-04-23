/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Math.hpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1588 $
 * Last Updated: $Date: 2008-08-23 09:36:02 -0700 (Sat, 23 Aug 2008) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/

#pragma once

#if (defined(USE_SSE) || defined(USE_SSE2))
  #include <xmmintrin.h>
#endif

namespace Bolitho
{
  namespace Math
  {
    class LIBRARY_API MathException : public Bolitho::Exception
    {
      DeclareRuntimeType();
    public:
      MathException(CONST Object* O, String S) : Exception(O)
      {}
    };


    CONST FLOAT PI = 3.1415926535f;
    CONST FLOAT TWOPI = 2.0f * 3.1415926535f;
    CONST FLOAT HALFPI = 0.5f * 3.1415926535f;
    CONST FLOAT QUARTERPI = 0.25f * 3.1415926535f;

    CONST FLOAT DEGREESTORADIANS = 0.01745329f;
    CONST FLOAT RADIANSTODEGREES = 57.2957795f; 
    

    // Conversion functions
    template<class Real>
    inline Real DegToRad(Real Degrees)
    { return Degrees * DEGREESTORADIANS; }

    template<class Real>
    inline Real RadToDeg(Real Radians)
    { return Radians * RADIANSTODEGREES; }

    // Trigonometric functions
    template<class Real>
    inline Real Sin(Real Rad);
    template<class Real>
    inline Real Cos(Real Rad);
    template<class Real>
    inline Real Tan(Real Rad);
    template<class Real>
    inline Real ASin(Real X);
    template<class Real>
    inline Real ACos(Real X);
    template<class Real>
    inline Real ATan(Real X);
    template<class Real>
    inline Real ATan2(Real Y, Real X);

    template <class Real>
    Real FastSin0(Real fAngle);
    template <class Real>
    Real FastSin1(Real fAngle);
    template <class Real>
    Real FastCos0(Real fAngle);
    template <class Real>
    Real FastCos1(Real fAngle);
    template <class Real>
    Real FastTan0(Real fAngle);
    template <class Real>
    Real FastTan1(Real fAngle);
    template <class Real>
    Real FastInvSin0(Real fValue);
    template <class Real>
    Real FastInvSin1(Real fValue);
    template <class Real>
    Real FastInvCos0(Real fValue);
    template <class Real>
    Real FastInvCos1(Real fValue);
    template <class Real>
    Real FastInvTan0(Real fValue);
    template <class Real>
    Real FastInvTan1(Real fValue);

    // Exponential / Logarithmic functions
    template<class Real>
    inline Real Exp(Real X);
    template<class Real>
    inline Real Log10(Real X);
    template<class Real>
    inline Real Ln(Real X);
    template<class Real>
    inline Real Log2(Real X);
    template<class Real>
    inline Real NextPowerOf2(Real X);
    template<class Real>
    inline BOOL IsPower2(Real X);
    template<class Real>
    inline Real Pow(Real X, Real Y);

    template<class Real>
    inline Real Abs(Real X);
    template<class Real>
    inline INT Sign(Real X);

    template<class Real>
    inline Real Sqrt(Real X);
    template<class Real>
    inline Real InvSqrt(Real X);
    template<class Real>
    inline Real Sqr(Real X);

    template<class Real>
    inline Real Ceil(Real X);
    template<class Real>
    inline Real Floor(Real X);

    template<class Real>
    inline Real Mod(Real A, Real B);

    template<class T>
    T RoundUp(T Value, T Granularity);
    template<class T>
    T RoundDown(T Value, T Granularity);

    template<class T>
    T UniformRandom();

    template<class T>
    T Random(T MaxValue);
    template<class T>
    T Random(T MinValue, T MaxValue);

    template<class T>
    VOID Shuffle(T* X, SIZE_T N, SIZE_T Iterations);

    template<class T>
    T KahanSum(T* pData, SIZE_T N);
    template<class T>
    VOID KahanSumStep(T& BigSum, T& SmallSum, T x);

    template<class T>
    inline Array<T> SolveLinear(T a1, T a0, T Epsilon);
    template<class T>
    inline Array<T> SolveQuadratic(T a2, T a1, T a0, T Epsilon);
    template<class T>
    inline Array<T> SolveCubic(T a1, T a0, T Epsilon);

    #include "MathImpl.hpp"

  }
}

