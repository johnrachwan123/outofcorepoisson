/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/PiecewisePolynomial.hpp $
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

#include "Utility/NumberTraits.hpp"
#include "Math/Math.hpp"
#include "Math/Polynomial.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<INT Degree, class T>
    class PiecewisePolynomialN
    {
    public:
      struct SEGMENT
      {
      public:
        PolynomialN<Degree,T> Polynomial;
        T Start;

        template<INT Degree2>
        typename PiecewisePolynomialN<Degree+Degree2,T>::SEGMENT Multiply(CONST typename PiecewisePolynomialN<Degree2,T>::SEGMENT& P) CONST;
        SEGMENT Scale(T s) CONST;
        SEGMENT Shift(T t) CONST;
        BOOL operator < (CONST SEGMENT& P) CONST;
      };

    public:
      PiecewisePolynomialN();
      PiecewisePolynomialN(CONST PiecewisePolynomialN& P);
      ~PiecewisePolynomialN();

      PiecewisePolynomialN& operator = (CONST PiecewisePolynomialN& P);
      template<INT Degree2>
      PiecewisePolynomialN& operator = (CONST PiecewisePolynomialN<Degree2,T>& P);

      SIZE_T SegmentCount() CONST;

      T Start() CONST;
      T End() CONST;

      PiecewisePolynomialN Clip(T Start, T End) CONST;

      VOID Resize(SIZE_T Count);
      SIZE_T Count() CONST;
      SEGMENT& operator[] (SIZE_T i);
      CONST SEGMENT& operator[] (SIZE_T i) CONST;

      VOID Optimize();


      T operator()(T x) CONST;
      T Evaluate(T x) CONST;
      T Integral(T Min, T Max) CONST;

      PiecewisePolynomialN  operator + (CONST PiecewisePolynomialN& p) CONST;
      PiecewisePolynomialN  operator - (CONST PiecewisePolynomialN& p) CONST;

      template<INT Degree2>
      PiecewisePolynomialN<Degree+Degree2,T> operator * (CONST PiecewisePolynomialN<Degree2,T>& P) CONST;
      template<INT Degree2>
      PiecewisePolynomialN<Degree+Degree2,T> operator * (CONST PolynomialN<Degree2,T>& P) CONST;

      PiecewisePolynomialN& operator += (T s);
      PiecewisePolynomialN& operator -= (T s);
      PiecewisePolynomialN& operator *= (T s);
      PiecewisePolynomialN& operator /= (T s);
      PiecewisePolynomialN  operator +  (T s) CONST;
      PiecewisePolynomialN  operator -  (T s) CONST;
      PiecewisePolynomialN  operator *  (T s) CONST;
      PiecewisePolynomialN  operator /  (T s) CONST;

      PiecewisePolynomialN& AddScaled(CONST PiecewisePolynomialN& P, T Scale);

      PiecewisePolynomialN Scale(T s) CONST;
      PiecewisePolynomialN Shift(T t) CONST;

      PiecewisePolynomialN<Degree-1,T> Derivative() CONST;
      PiecewisePolynomialN<Degree+1,T> Integral() CONST;

      Array<T> GetSolutions(T c, T Epsilon, T Min = RealTraits<T>::MinValue, T Max = RealTraits<T>::MaxValue) CONST;

      PiecewisePolynomialN<Degree+1,T> MovingAverage(T radius);

      static PiecewisePolynomialN ConstantFunction(T Width = (T)0.5);
      static PiecewisePolynomialN GaussianApproximation(T Width = (T)0.5);

    private:
      Array<SEGMENT> m_Segments;

    };

#include "PiecewisePolynomialImpl.hpp"
  }
}
