/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Matrix.hpp $
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

#include "Math/Math.hpp"
#include "Math/Vector.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<class T>
		class Matrix : public Object
    {
    public:
      Matrix();
      Matrix(CONST Matrix& M);
      Matrix(SIZE_T N, SIZE_T M);
      Matrix(SIZE_T N, SIZE_T M, T* pM);
      virtual ~Matrix();

      // Storage management
      SIZE_T Rows() CONST;
      SIZE_T Columns() CONST;
      VOID Resize(SIZE_T N, SIZE_T M);

      // Element access
      CONST T& operator () (SIZE_T i, SIZE_T j) CONST;
      T& operator () (SIZE_T i, SIZE_T j);
      CONST T& operator [] (SIZE_T ij) CONST;
      T& operator [] (SIZE_T ij);

      // Get / Set elements
      Vector<T> GetRow(SIZE_T i) CONST;
      Vector<T> GetColumn(SIZE_T j) CONST;
      Vector<T> GetDiagonal(INT n) CONST;
      T         GetElement(SIZE_T i, SIZE_T j) CONST;

      VOID SetRow(SIZE_T i, CONST Vector<T>& V);
      VOID SetRow(SIZE_T i, T& V);
      VOID SetColumn(SIZE_T j, CONST Vector<T>& V);
      VOID SetColumn(SIZE_T j, T& V);
      VOID SetDiagonal(INT n, CONST Vector<T>& V);
      VOID SetDiagonal(INT n, T& V);
      VOID SetElement(SIZE_T i, SIZE_T j, T& V);

      VOID SetZero();
      VOID SetIdentity();

      Matrix<T> operator + (CONST Matrix<T>& M) CONST;
      Matrix<T> operator - (CONST Matrix<T>& M) CONST;
      Matrix<T> operator * (CONST Matrix<T>& M) CONST;
      Vector<T> operator * (CONST Vector<T>& V) CONST;
      Matrix<T> operator * (CONST T& V) CONST;
      Matrix<T> operator / (CONST T& V) CONST;

      Matrix<T>& operator *= (CONST T& V);
      Matrix<T>& operator /= (CONST T& V);
      Matrix<T>& operator += (CONST Matrix<T>& M);
      Matrix<T>& operator -= (CONST Matrix<T>& M);
      Matrix<T>& operator += (T V);
      Matrix<T>& operator -= (T V);


      T Norm(SIZE_T Ln) CONST;
      VOID Normalize();

      Matrix<T> Inverse() CONST;
      Matrix<T> Transpose() CONST;
      T Determinant() CONST;

      VOID WriteBinary(CONST TCHAR* Filename) CONST;
      VOID ReadBinary(CONST TCHAR* Filename);

    protected:
      VOID Deallocate();

      SIZE_T m_N;
      SIZE_T m_M;
      T* m_pM;

    };

    template <class T>
    Matrix<T> operator* (T fScalar, CONST Matrix<T>& V);

    template <class T>
    Matrix<T> operator/ (T fScalar, CONST Matrix<T>& V);

    template<class T>
    IO::Stream& operator << (IO::Stream& S, CONST Matrix<T>& M);

    typedef Matrix<FLOAT> MatrixF;
    typedef Matrix<DOUBLE> MatrixD;
    typedef Matrix<INT> MatrixI;

#include "MatrixImpl.hpp"
  }
}

