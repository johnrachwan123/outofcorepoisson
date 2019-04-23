/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/Math/Matrix4.hpp $
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

#include "Math/Quaternion.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"
#include "Math/MatrixN.hpp"

namespace Bolitho
{
  namespace Math
  {
    template<class T>
    class Matrix4 : public MatrixN<4,T>
    {
      using MatrixN<4,T>::m_M;
    public:
      using MatrixN<4,T>::operator*;

      Matrix4();
      Matrix4(CONST MatrixN<3,T>& M);
      Matrix4(CONST MatrixN<4,T>& M);
      Matrix4(CONST T* pM);
      Matrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33);
      Matrix4(CONST VectorN<3,T>& Translation, CONST Quaternion<T>& Rotation);  
      
      ~Matrix4();
      
      Matrix3<T> GetSubMatrix(SIZE_T i) CONST;
      VectorN<3,T> GetTranslation() CONST;
      Matrix3<T> GetRotation() CONST;
      VOID SetSubMatrix(SIZE_T i, CONST Matrix3<T>& M);
      VOID SetTranslation(CONST VectorN<3,T>& V);
      VOID SetRotation(CONST Matrix3<T>& R);
      
      VectorN<3,T> operator * (CONST VectorN<3,T>& V) CONST;
      
      // Math operations
      Matrix4<T> Inverse() CONST;
      T Determinant() CONST;
      T Trace() CONST;
      Matrix4<T> Adjoint() CONST;
      
      // Geometric operations
      static Matrix4<T> PerspectiveFovLH(T FovY, T Aspect, T NearPlane, T FarPlane);
      static Matrix4<T> PerspectiveFovRH(T FovY, T Aspect, T NearPlane, T FarPlane);
      
      static Matrix4<T> LookAtLH(VectorN<3,T> Position, VectorN<3,T> Ref, VectorN<3,T> Up);
      static Matrix4<T> LookAtRH(VectorN<3,T> Position, VectorN<3,T> Ref, VectorN<3,T> Up);
      
      static Matrix4<T> OrthoLH(T w, T h, T zNear, T zFar);
      
      static Matrix4<T> Scale(T Sx, T Sy, T Sz, T Sw = 1);
      
      static Matrix4<T> Translation(T Tx, T Ty, T Tz, T Tw = 1);
      static Matrix4<T> Translation(CONST VectorN<3,T>& V);
      static Matrix4<T> Translation(CONST VectorN<4,T>& V);
      
      static Matrix4<T> Rotation(T Angle, VectorN<3,T>& Axis);
      static Matrix4<T> Rotation(T Angle, T Ax, T Ay, T Az);
      
      String ToString() CONST;
      
#if defined(USE_LUA)
      static Lua::Bind::Scope GenerateBindings(LPCTSTR Name);
#endif
      
    };
    
#include "Matrix4Impl.hpp"
    
    typedef Matrix4<FLOAT> Matrix4F;
    typedef Matrix4<DOUBLE> Matrix4D;
  }
}
