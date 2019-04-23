/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Matrix2.hpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1631 $
 * Last Updated: $Date: 2008-09-15 07:52:23 -0700 (Mon, 15 Sep 2008) $
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

#include "Math/Vector2.hpp"
#include "Math/Math.hpp"

namespace Bolitho
{
  namespace Math
  {
      template<class T>
      class Matrix2
      {
      public:
        Matrix2();
        Matrix2(CONST Matrix2& M);
        Matrix2(CONST T* pM);
        Matrix2(T m00, T m01, T m10, T m11);
        ~Matrix2();

        // Storage management
        SIZE_T Rows() CONST;
        SIZE_T Columns() CONST;

        // Element access
        CONST T& operator () (SIZE_T i, SIZE_T j) CONST;
        T& operator () (SIZE_T i, SIZE_T j);
        CONST T& operator [] (SIZE_T ij) CONST;
        T& operator [] (SIZE_T ij);

        // Get / Set elements
        Vector2<T> GetRow(SIZE_T i) CONST;
        Vector2<T> GetColumn(SIZE_T j) CONST;
        Vector2<T> GetDiagonal() CONST;
        T          GetElement(SIZE_T i, SIZE_T j) CONST;

        VOID SetRow(SIZE_T i, CONST Vector2<T>& V);
        VOID SetRow(SIZE_T i, T& V);
        VOID SetColumn(SIZE_T j, CONST Vector2<T>& V);
        VOID SetColumn(SIZE_T j, T& V);
        VOID SetDiagonal(CONST Vector2<T>& V);
        VOID SetDiagonal(CONST T& V);
        VOID SetDiagonal(T D0, T D1);
        VOID SetElement(SIZE_T i, SIZE_T j, T& V);

        VOID SetZero();
        VOID SetIdentity();

        // Arithmetic operations
        Matrix2<T> operator + (CONST Matrix2<T>& M) CONST;
        Matrix2<T> operator - (CONST Matrix2<T>& M) CONST;
        Matrix2<T> operator * (CONST Matrix2<T>& M) CONST;
        Vector2<T> operator * (CONST Vector2<T>& V) CONST;
        Matrix2<T> operator * (CONST T& V) CONST;
        Matrix2<T> operator / (CONST T& V) CONST;

        Matrix2<T>& operator *= (CONST T& V);
        Matrix2<T>& operator /= (CONST T& V);
        Matrix2<T>& operator += (CONST Matrix2<T>& M);
        Matrix2<T>& operator -= (CONST Matrix2<T>& M);

        // Norms / Normalisation
        T Norm(SIZE_T Ln) CONST;
        VOID Normalize();

        // Math operations
        Matrix2<T> Inverse() CONST;
        Matrix2<T> Transpose() CONST;
        T Determinant() CONST;
        T Trace() CONST;
        VOID EigenDecomposition (Matrix2<T>& R, Matrix2<T>& D) CONST;

        // Commonly used matrices
        static CONST Matrix2<T> Zero;
        static CONST Matrix2<T> Identity;

        // Geometric operations
        static Matrix2<T> Rotation(T Rads);
        static Matrix2<T> Scale(T Sx, T Sy);
      protected:
        T m_M[4];
      };

      // global operators
      template <class T>
      Matrix2<T> operator * (T fScalar, CONST Matrix2<T>& M);
      template <class T>
      Matrix2<T> operator / (T fScalar, CONST Matrix2<T>& M);
      template <class T>
      Matrix2<T> operator * (CONST Vector2<T>& V, CONST Matrix2<T>& M);

      typedef Matrix2<FLOAT> Matrix2F;
      typedef Matrix2<DOUBLE> Matrix2D;
      typedef Matrix2<INT> Matrix2I;

#include "Matrix2Impl.hpp"
  }
}
