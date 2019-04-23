/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Matrix3.hpp $
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

#include "Math/Vector3.hpp"
#include "Math/MatrixN.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<class T>
    class Matrix3 : public MatrixN<3,T>
    {
      using MatrixN<3,T>::m_M;
    public:
      Matrix3();
      Matrix3(CONST Matrix3<T>& M);
      Matrix3(CONST MatrixN<3,T>& M);
      Matrix3(CONST T* pM);
      Matrix3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22);
      
      template<class U>
      Matrix3(CONST Matrix3<U>& M);
      template<class U>
      Matrix3(CONST MatrixN<3,U>& M);
      
      ~Matrix3();

      VOID SetDiagonal(T D0, T D1, T D2);

      Matrix3<T> Inverse() CONST;
      T Determinant() CONST;
      T Trace() CONST;

      VOID EigenDecomposition (Matrix3<T>& R, Matrix3<T>& D) CONST;

      // String operations
      String ToString() CONST;

      static Matrix3<T> Rotation(T Angle, VectorN<3,T>& Axis);
      static Matrix3<T> RotationX(T Angle);
      static Matrix3<T> RotationY(T Angle);
      static Matrix3<T> RotationZ(T Angle);

      static Matrix3<T> Scale(FLOAT x, FLOAT y, FLOAT z);
      static Matrix3<T> Scale(CONST VectorN<3,T>& T);

    protected:
      BOOL Tridiagonalize(T Diag[3], T Subd[2]);
      BOOL QLAlgorithm(T Diag[3], T Subd[2]);
    };

    typedef Matrix3<FLOAT> Matrix3F;
    typedef Matrix3<DOUBLE> Matrix3D;
    typedef Matrix3<INT> Matrix3I;

    #include "Matrix3Impl.hpp"
  }
}
