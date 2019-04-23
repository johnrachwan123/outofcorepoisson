/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/MatrixN.hpp $
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

#include "Math/VectorN.hpp"
#include "Math/Math.hpp"

#include "IO/TextStream.hpp"
#include "IO/BinaryStream.hpp"
#include "IO/File.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<SIZE_T N, class T>
    class MatrixN
    {
    public:
      MatrixN();
      MatrixN(CONST MatrixN& M);
      MatrixN(CONST T* pM);
      ~MatrixN();

      // Storage management
      SIZE_T Rows() CONST;
      SIZE_T Columns() CONST;

      // Element access
      CONST T& operator () (SIZE_T i, SIZE_T j) CONST;
      T& operator () (SIZE_T i, SIZE_T j);
      operator CONST T* () CONST;
      operator T* ();

      // Get / Set elements
      VectorN<N,T> GetRow(SIZE_T i) CONST;
      VectorN<N,T> GetColumn(SIZE_T j) CONST;
      VectorN<N,T> GetDiagonal() CONST;
      T            GetElement(SIZE_T i, SIZE_T j) CONST;

      VOID SetRow(SIZE_T i, CONST VectorN<N,T>& V);
      VOID SetRow(SIZE_T i, T V);
      VOID SetColumn(SIZE_T j, CONST VectorN<N,T>& V);
      VOID SetColumn(SIZE_T j, T V);
      VOID SetDiagonal(CONST VectorN<N,T>& V);
      VOID SetDiagonal(T V);
      VOID SetElement(SIZE_T i, SIZE_T j, T V);

      VOID SetZero();
      VOID SetIdentity();

      static MatrixN<N,T> Identity();

      // Arithmetic operations
      MatrixN<N,T> operator + (CONST MatrixN<N,T>& M) CONST;
      MatrixN<N,T> operator - (CONST MatrixN<N,T>& M) CONST;
      MatrixN<N,T> operator * (CONST MatrixN<N,T>& M) CONST;
      VectorN<N,T> operator * (CONST VectorN<N,T>& V) CONST;
      MatrixN<N,T> operator * (T V) CONST;
      MatrixN<N,T> operator / (T V) CONST;

      MatrixN<N,T>& operator *= (T V);
      MatrixN<N,T>& operator *= (CONST MatrixN<N,T>& M);
      MatrixN<N,T>& operator /= (T V);
      MatrixN<N,T>& operator += (CONST MatrixN<N,T>& M);
      MatrixN<N,T>& operator -= (CONST MatrixN<N,T>& M);

      // Norms / Normalisation
      T Norm(UINT Ln) CONST;
      VOID Normalize();

      // Math operations
      MatrixN<N,T> Transpose() CONST;
      VOID EigenDecomposition (MatrixN<N,T>& R, MatrixN<N,T>& D) CONST;
      VOID EigenDecomposition (VectorN<N,T> EigenVectors[N], VectorN<N,T>& EigenValues) CONST;

      // IO operations
      static MatrixN<N,T> Read(LPCTSTR Filename);
      VOID Write(LPCTSTR Filename);
    protected:
      T m_M[N*N];
    };

    // global operators
    template<SIZE_T N, class T>
    MatrixN<N,T> operator * (T Scalar, CONST MatrixN<N,T>& M);
    template<SIZE_T N, class T>
    MatrixN<N,T> operator / (T Scalar, CONST MatrixN<N,T>& M);
    template<SIZE_T N, class T>
    MatrixN<N,T> operator * (CONST VectorN<N,T>& V, CONST MatrixN<N,T>& M);

    #include "MatrixNImpl.hpp"

		template<SIZE_T N, class T>
    IO::BinaryStream& operator << (IO::BinaryStream& Stream, CONST MatrixN<N,T>& Matrix);
		template<SIZE_T N, class T>
    IO::BinaryStream& operator >> (IO::BinaryStream& Stream, MatrixN<N,T>& Matrix);

		template<SIZE_T N, class T>
		IO::TextStream& operator << (IO::TextStream& Stream, CONST MatrixN<N,T>& Matrix);
		template<SIZE_T N, class T>
		IO::TextStream& operator >> (IO::TextStream& Stream, MatrixN<N,T>& Matrix);

  }
}
